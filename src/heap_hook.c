#include "heap_hook.h"
#include "global_operation.h"
#include <stdint.h>
#include <string.h>           // For memset, memcpy

void *chunk_alloc_hook(struct thread_cache *tc, size_t size,
                               size_t align, int flag)
{
        void *ret = NULL;
        struct chunk_head *ch = NULL;
        uint8_t kind;
        uint8_t num = check_size(size, &kind);

        ch = get_suitable_chunk(tc, kind, num, align, NULL);
        ch->seek = size;
        ret = ch + 1;
        
        if (flag)        // calloc
                memset(ret, 0, size);

        return ret;
}

void *chunk_realloc_hook(struct thread_cache *tc, void *ptr, size_t size)
{
        void *ret = NULL;
        struct chunk_head *old_ch = ptr - chunk_head_size;
        struct chunk_head *new_ch = NULL;
        uint8_t kind;
        uint8_t num = check_size(size, &kind);
        
        new_ch = get_suitable_chunk(tc, kind, num, 0, old_ch);
        if (new_ch == old_ch) {
                ret = ptr;
        } else {
                ret = new_ch + 1;
                memcpy(ret, ptr, old_ch->seek);
                do_chunk_free(find_central_of_pointer(tc, old_ch), old_ch);
        }

        new_ch->seek = size;
        return ret;
}

struct central_cache *find_central_of_pointer(struct thread_cache *tc,
                                              void *ptr)
{
        struct central_cache *cc = tc->cc;
        while (cc != NULL) {
                if (ptr>(void*)cc && ptr<(void*)cc+central_cache_size)
                        break;
                cc = cc->next;
        }
        return cc;
}

void do_chunk_free(struct central_cache *cc,struct chunk_head *ch)
{
        printf("do free\n");
        struct chunk_head *prev_ch = cc->free_chunk;
        struct chunk_head *next_ch = prev_ch;
        ch->seek = chunk_size[ch->kind] * ch->num;
        ch->next = NULL;

        for (; next_ch!=NULL; prev_ch=next_ch, next_ch=next_ch->next) {
                if (next_ch > ch)
                        break;
        }

        /* Check arround target chunk */
        if (next_ch == cc->free_chunk) {
                cc->free_chunk = ch;
        } else {
                if ((size_t)ch-(size_t)prev_ch == prev_ch->seek) {
                        prev_ch->seek +=  ch->seek;
                        ch = prev_ch;
                } else {
                        ch->next = prev_ch->next;
                        prev_ch->next = ch;
                }
        }
                
        if (next_ch != NULL) {
                if ((size_t)next_ch-(size_t)ch == ch->seek) {
                        ch->seek += next_ch->seek;
                        ch->next = next_ch->next;
                } else {
                        ch->next = next_ch;
                }
        }
}

static uint8_t check_size(size_t size, uint8_t *kind)
{
        int index;
        for (index=0; index<num_of_kinds-1; ++index) {
                if (size <= (3*chunk_size[index] - chunk_head_size)) {
                        break;
                }
        }
        *kind = index;
        return (size+chunk_head_size)/chunk_size[index] + 1;
}

static void *get_suitable_chunk(struct thread_cache *tc,
                                uint8_t kind, uint8_t num, size_t align,
                                struct chunk_head *old_ch)
{
        struct chunk_head *ch = NULL;
        struct chunk_head *prev_ch = NULL;
        struct central_cache *cc = NULL;
        size_t tar_size = chunk_size[kind] * num;
        size_t ali_num;
        int index;

        if (old_ch != NULL) {           // realloc
                cc = find_central_of_pointer(tc, old_ch);
                if (old_ch->kind == kind) {                     // Same kind
                        if (old_ch->num == num) {           // Same num
                                return old_ch;
                        } else if (old_ch->num > num) {     // Less num
                                ch = (void*)old_ch + tar_size;
                                ch->kind = kind;
                                ch->num = old_ch->num - num;
                                do_chunk_free(cc,ch);
                                
                                old_ch->num = num;
                                return old_ch;
                        } else {                            // More num
                                /* Check if it is free behind old chunk */
                                ch = get_appoint_chunk(cc,
                                                       tar_size -
                                                       chunk_size[kind] *
                                                       old_ch->num,
                                                       (void*)old_ch +
                                                       old_ch->num *
                                                       chunk_size[kind]);
                                if (ch != NULL) {
                                        old_ch->num = num;
                                        return old_ch;
                                }
                        }
                } else if (old_ch->kind > kind) {               // Smaller kind
                        /* Turn to new kind */
                        for (index=kind; index<old_ch->kind; ++index)
                                old_ch->num *= 4;
                        old_ch->kind = kind;

                        ch = (void*)old_ch + tar_size;
                        ch->kind = kind;
                        ch->num = old_ch->num - num;
                        do_chunk_free(cc, ch);

                        old_ch->num = num;
                        return old_ch;
                }
        }

        if (tc->cc == NULL)
                thread_add_central(tc);
        cc = tc->cc;
        /* Find in centrals */
        while(1) {
                prev_ch = cc->free_chunk;
                ch = prev_ch;
                /* Find in free chunks */
                for (; ch != NULL; prev_ch=ch, ch=ch->next) {
                        if (ch->seek >= tar_size)
                                break;
                }
                if (ch != NULL)
                        break;
                if (cc->next == NULL) {
                        thread_add_central(tc);
                        cc = tc->cc;
                } else {
                        cc = cc->next;
                }
        }

        /* If the size of target chunk is bigger than need */
        if (ch->seek != tar_size) {
                if (ch == cc->free_chunk) {
                        cc->free_chunk = (void*)ch + tar_size;
                        cc->free_chunk->seek = ch->seek - tar_size;
                        cc->free_chunk->next = ch->next;
                } else {
                        prev_ch->next = (void*)ch + tar_size;
                        prev_ch->next->seek = ch->seek - tar_size;
                        prev_ch->next->next = ch->next;
                }
        } else {
                if (ch == cc->free_chunk)
                        cc->free_chunk = ch->next;
                else
                        prev_ch->next = ch->next;
        }

        /* Initialize final target chunk */
        ch->kind = kind;
        ch->num = num;
        return ch;
}

static void *get_appoint_chunk(struct central_cache *cc,
                               size_t tar_size, void *ptr)
{
        struct chunk_head *ch = cc->free_chunk;
        struct chunk_head *prev_ch = ch;

        /* Search in free chunk */
        for(; ch!=NULL; prev_ch=ch, ch=ch->next) {
                if (ch==ptr && ch->seek>=tar_size)
                        break;
        }

        /* Can not get appoint chunk */
        if (ch == NULL)
                return ch;

        /* If the size of target chunk is bigger than need */
        if (ch->seek != tar_size) {
                if (ch == cc->free_chunk) {
                        cc->free_chunk = (void*)ch + tar_size;
                        cc->free_chunk->seek = ch->seek - tar_size;
                } else {
                        prev_ch->next = (void*)ch + tar_size;
                        prev_ch->next->seek = ch->seek - tar_size;
                }
        } else {
                if (ch == cc->free_chunk)
                        cc->free_chunk = ch->next;
                else
                        prev_ch->next = ch->next;
        }
        return ch;
}

