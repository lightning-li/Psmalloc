#include "psmalloc.h"
#include "common.h"
#include "global_operation.h"
#include "heap_hook.h"
#include "mmap_hook.h"

void *ps_malloc(size_t size) __THROW
{
        void *ret = NULL;
        struct thread_cache *current_thread = get_current_thread();

        current_thread->count++;
        if (size > critical_size) {
                // Third argument is zero
                // Bytes in chunk will not be initialized
                ret = chunk_alloc_hook(current_thread, size, 0);
        } else {
                ret = mmap_alloc_hook(current_thread, size, 0);
        }
        return ret;
}

void *ps_calloc(size_t n, size_t size) __THROW
{
        void *ret = NULL;
        struct thread_cache *current_thread = get_current_thread();

        current_thread->count++;
        if (n*size > critical_size) {
                // Third argument is one
                // Bytes in chunk will be initialized
                ret = chunk_alloc_hook(current_thread, size*n, 1);
        } else {
                ret = mmap_alloc_hook(current_thread, size*n, 1);
        }
        return ret;
}

void *ps_realloc(void *ptr, size_t size) __THROW
{
        void *ret = NULL;
        struct thread_cache *current_thread = get_current_thread();

        current_thread->count++;
        if (size < critical_size) {
                ret = chunk_realloc_hook(tc, ptr, size);
        } else {
                ret = mmap_realloc_hook(tc, ptr, size);
        }
        return ret;
}

void ps_free(void *ptr) __THROW
{
        struct central_cache *cc = NULL;
        struct thread_cache *current_thread = get_current_thread();
        current_thread->count--;
        
        cc = find_central_of_pointer(current_thread, ptr);
        if (cc != NULL)
                do_chunk_free(cc, ptr - chunk_head_size);
        else
                do_mmap_free(tc, ptr - mmap_head_size);

        // Check if this thread till be used
        check_thread_use();
}
