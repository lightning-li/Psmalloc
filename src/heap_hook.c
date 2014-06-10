#include "heap_hook.h"
#include "global_operation.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

void *chunk_alloc_hook(struct thread_cache *tc, size_t size, int flag)
{
        void *ret = NULL;
        struct chunk_head *ch = NULL;
        enum chunk_kind kind;
        uint32_t num = check_size(size, &kind);

        ch = get_suitable_chunk(tc, kind, num, NULL);
        ch->seek = size;
        ret = ch + 1;

        // calloc
        if (flag)
                memset(ret, 0, size);
        return ret;
}

void *chunk_realloc_hook(struct thread_cache *tc, void *ptr, size_t size)
{
        void *ret = NULL;
        struct chunk_head *old_ch = ptr - chunk_head_size;
        struct chunk_head *new_ch = NULL;
        enum chunk_kind kind;
        uint32_t num = check_size(size, &kind);

        new_ch = get_suitable_chunk(tc, kind, num, old_ch);
        
        if (new_ch == old_ch) {     // Same address
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
        for (; cc!=NULL; cc=cc->next)
                if (ptr<cc->end && ptr>=cc->start)
                        break;
        return cc;
}

void do_chunk_free(struct central_cache *cc,struct chunk_head *ch)
{
        struct chunk_head *prev_ch = cc->free_chunk;
        struct chunk_head *next_ch = prev_ch;
        ch->num = chunk_trans[ch->kind] * ch->num;
        ch->seek = (size_t)ch + ch->num * chunk_size[0] - 1;

        for (; next_ch!=NULL; prev_ch=next_ch, next_ch=next_ch->next) {
                if (next_ch > ch)
                        break;
        }

        // Check arround target chunk
        // Piece front
        if (next_ch == cc->free_chunk) {
                cc->free_chunk = ch;
        } else {
                if (prev_ch->seek == (size_t)ch-1) {
                        prev_ch->num += ch->num;
                        prev_ch->seek += ch->num * chunk_size[0];
                        ch = prev_ch;
                } else {
                        prev_ch->next = ch;
                }
        }

        // Piece behind
        if (next_ch != NULL) {
                if (ch->seek == (size_t)next_ch-1) {
                        ch->num += next_ch->num;
                        ch->seek += next_ch->num * chunk_size[0];
                        ch->next = next_ch->next;
                } else {
                        ch->next = next_ch;
                }
        }
}

static uint32_t check_size(size_t size, enum chunk_kind *kind)
{
        int index;
        for (index=0; index<4; ++index) {
                if (size <=
                    (3*chunk_size[index] - chunk_head_size)) {
                        *kind = index;
                        return (size+chunk_head_size)/chunk_size[index] + 1;
                }
        }
        *kind = index;
        return size/chunk_size[index] + 1;
}

static void *get_suitable_chunk(struct thread_cache *tc,
                                enum chunk_kind kind,
                                uint32_t num, struct chunk_head *old_ch)
{
        struct chunk_head *ch = NULL;
        struct chunk_head *prev_ch = NULL;
        struct central_cache *cc = NULL;
        uint32_t target_ti_num = chunk_trans[kind] * num;
        int index;

        // realloc
        if (old_ch != NULL) {
                cc = find_central_of_pointer(tc, old_ch);
                if (old_ch->kind == kind) {                     // Same kind
                        if (old_ch->num == num) {           // Same num
                                return old_ch;
                        } else if (old_ch->num > num) {     // Less num
                                ch = (void*)old_ch +
                                        target_ti_num*chunk_size[0];
                                ch->kind = kind;
                                ch->num = old_ch->num - num;
                                do_chunk_free(cc,ch);
                                
                                // Change old chunk
                                old_ch->num = num;
                                return old_ch;
                        } else {                            // More num
                                // Check if it is free behind old chunk
                                ch = get_appoint_chunk(cc,
                                                       chunk_trans[kind] *
                                                       (num - old_ch->num),
                                                       (void*)old_ch +
                                                       chunk_size[kind] *
                                                       old_ch->num);
                                if (ch != NULL) {
                                        old_ch->num = num;
                                        return old_ch;
                                }
                        }
                } else if (old_ch->kind > kind) {               // Smaller kind
                        // Turn to new kind
                        for (index=kind; index>old_ch->kind; --index)
                                old_ch->num *= 4;
                        old_ch->kind = kind;

                        ch = (void*)old_ch + target_ti_num*chunk_size[0];
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
        // Find in central caches
        while (1) {
                ch = cc->free_chunk;
                prev_ch = cc->free_chunk;
                // Find in free chunks
                for (; ch != NULL; prev_ch=ch, ch=ch->next) {
                        if (ch->num >= target_ti_num)
                                break;
                }
                if (ch != NULL)
                        break;
                if (cc->next == NULL){
                        thread_add_central(tc);
                }
                cc = cc->next;
        }

        // If the size of target chunk is bigger than need
        if (ch->num != target_ti_num) {
                if (ch == cc->free_chunk) {
                        cc->free_chunk = (void*)ch +
                                target_ti_num * chunk_size[0];
                        cc->free_chunk->num = ch->num - target_ti_num;
                        cc->free_chunk->seek = ch->seek;
                } else {
                        prev_ch->next = (void*)ch +
                                target_ti_num * chunk_size[0];
                        prev_ch->next->num = ch->num - target_ti_num;
                        prev_ch->next->seek = ch->seek;
                }
        } else {
                if (ch == cc->free_chunk)
                        cc->free_chunk = ch->next;
                else
                        prev_ch->next = ch->next;
        }

        // Initialize final target chunk
        ch->kind = kind;
        ch->num = num;
        return ch;
}

static void *get_appoint_chunk(struct central_cache *cc,
                               uint32_t ti_num, void *ptr)
{
        struct chunk_head *ch = cc->free_chunk;
        struct chunk_head *prev_ch = ch;

        // Search in free chunk
        for(; ch!=NULL; prev_ch=ch, ch=ch->next) {
                if (ch==ptr && ch->num>=ti_num)
                        break;
        }

        // Can not get appoint chunk
        if (ch == NULL)
                return ch;

        // If the size of target chunk is bigger than need
        if (ch->num != ti_num) {
                if (ch == cc->free_chunk) {
                        cc->free_chunk = (void*)ch + ti_num*chunk_size[0];
                        cc->free_chunk->num = ch->num - ti_num;
                        cc->free_chunk->seek = ch->seek;
                } else {
                        prev_ch->next = (void*)ch + ti_num*chunk_size[0];
                        prev_ch->next->num = ch->num - ti_num;
                        prev_ch->next->seek = ch->seek;
                }
        } else {
                if (ch == cc->free_chunk)
                        cc->free_chunk = ch->next;
                else
                        prev_ch->next = ch->next;
        }
        return ch;
}

