#include "mmap_hook.h"
#include "global_operation.h"
#include "heap_hook.h"
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

void *mmap_alloc_hook(struct thread_cache *tc, size_t size, int flag)
{
        void *ret = NULL;
        struct chunk_head *mm = NULL;
        struct chunk_head *mm_prev = tc->mm;

        /* Get mmap */
        mm = mmap(NULL, size + chunk_head_size, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANON, -1, 0);
        mm->seek = size;
        mm->next = NULL;

        /* Add mmap chunk into thread */
        mm->next = tc->mm;
        tc->mm = mm;
        
        ret = mm + 1;
        if (flag)             // realloc
                memset(ret, 0, size);
        printf("mmap alloc\n");
        return ret;
}

void *mmap_realloc_hook(struct thread_cache *tc, void *ptr, size_t size)
{
        struct chunk_head *new_mm = NULL;
        // It could be in mmap or central
        struct chunk_head *old_mm = ptr - chunk_head_size;

        if (old_mm->seek >= size)
                return ptr;
        
        new_mm = mmap(NULL, size + chunk_head_size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANON, -1, 0);
        // Copy data
        memcpy(new_mm + 1, ptr, old_mm->seek);
        new_mm->next = tc->mm;
        tc->mm = new_mm;
        
        // Release old
        if ((int)old_mm > sbrk(0))    // Old pointer is in mmap
                do_mmap_free(tc, old_mm);
        else                     // Old pointer is in heap
                do_chunk_free(find_central_of_pointer(tc, old_mm), old_mm);

        return new_mm + 1;
}

void do_mmap_free(struct thread_cache *tc, struct chunk_head *old_mm)
{
        struct chunk_head *mm;
        if (tc->mm == old_mm) {
                tc->mm = old_mm->next;
        } else {
                mm = tc->mm;
                while(mm->next != old_mm)
                        mm = mm->next;
                mm->next = old_mm->next;
        }
        munmap(old_mm, old_mm->seek + chunk_head_size);
        printf("mmap free\n");
}
