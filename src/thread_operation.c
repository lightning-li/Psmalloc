#include "thread_operation.h"

enum chunk_kind check_size(size_t size)
{
        if (size <= (tiny_chunk_size - chunk_head_size))
                return tiny;
        else if (size <= (small_chunk_size - chunk_head_size))
                return small;
        else if (size <= (medium_chunk_size - chunk_head_size))
                return medium;
        else if (size <= (big_chunk_size - chunk_head_size))
                return big;
        else
                return huge;
}

void *chunk_hook(struct thread_cache *tc, size_t size, int flag)
{
        void *ret = NULL;
        size_t *size_ptr = NULL;
        struct chunk_head *ch = NULL;
        enum chunk_kind kind = check_size(size);

        ch = get_suitable_chunk(tc, kind, NULL);
        ch->seek = size;
        ret = ch + 1;
        // Check if it is neccessary to initialize to zero
        if (flag) {
                size_ptr = ret;
                for (size=size/sizeof(size_t)+1; size>0; --size)
                        *(size_ptr++) = 0;
        }
        return ret;
}

void *chunk_realloc_hook(struct thread_cache *tc, void *ptr, size_t size)
{
        size_t *old_size_ptr = ptr;
        size_t *new_size_ptr = NULL;
        struct chunk_head *old_ch = ptr - chunk_head_size;
        struct chunk_head *new_ch = NULL;
        size_t old_size = ch->seek;
        enum chunk_kind kind = check_size(size);

        // Same chunk
        if (kind == ch->kind)
                return ptr;

        new_ch = get_suitable_chunk(tc, kind, old_ch);
        new_ptr = new_ch + 1;
        // From the same address
        if (new_ch == old_ch) {
                new_ch->seek = size;
                return ptr;
        }

        // Copy data from old chunk to new chunk
        size = (size>old_size)?old_size:size;
        for (size=size/sizeof(size_t)+1; size>0; --size)
                *(new_ptr++) = *(old_ptr++);
        return new_ch+1;
}

static void *get_suitable_chunk(struct thread_cache *tc, \
                                enum chunk_kind kind, struct chunk_head *ch)
{
        
}
