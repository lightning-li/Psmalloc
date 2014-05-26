#include "common.h"
#include "heap_alloc.h"
#include "mmap_alloc.h"
#include "global_operation.h"
#include "thread_operation.h"

void *do_heap_malloc(size_t size)
{
        void *ret = NULL;
        struct thread_cache *current_thread = NULL;

        // Get thread cache for current thread
        current_thread = get_current_thread();

        //Get new chunk in current thread cache
        // Third argument zero, bytes in chunk will not be initialized
        ret = chunk_alloc_hook(current_thread, size, 0);
        return ret;
}

void *do_heap_calloc(size_t n, size_t size)
{
        void *ret = NULL;
        struct thread_cache *current_thread = NULL;

        // Get thread cache for current thread
        current_thread = get_current_thread();

        //Get new chunk in current thread cache
        // Third argument one, bytes in chunk will be initialized to zero
        ret = chunk_alloc_hook(current_thread, n*size, 1);
        return ret;
}

void *do_heap_realloc(void *ptr, size_t size)
{
        void *ret = NULL;
        struct thread_cache *current_thread = NULL;
        
        // Get thread cache for current thread
        current_thread = get_current_thread();

        ret = chunk_realloc_hook(current_thread, ptr, size);
        return ret;
}

void do_free(void *ptr)
{
        struct central_cache *cc = NULL;
        struct thread_cache *current_thread = NULL;

        // Get thread cache for current thread
        current_thread = get_current_thread();

        cc = find_central_of_pointer(current_thread, ptr);
        if (cc != NULL)
                add_free_chunk(cc, ptr);
        else
                do_mmap_free(ptr);
}
