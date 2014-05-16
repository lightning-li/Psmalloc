#include "common.h"
#include "heap_alloc.h"
#include "init.h"

void *do_heap_malloc(size_t size)
{
        void *ret = NULL;
        struct thread_cache_struct *current_thread = NULL;

        // Get thread cache for current thread
        current_thread = get_current_thread_cache(tkey);

        //Get new chunk in current thread cache
        // Third argument zero, bytes in chunk will not be initialized
        ret = chunk_hook(current_thread, size, 0);
        return ret;

}

void *do_heap_calloc(size_t n, size_t size)
{
        void *ret = NULL;
        struct thread_cache_struct *current_thread = NULL;

        // Get thread cache for current thread
        current_thread = get_current_thread_cache(tkey);

        //Get new chunk in current thread cache
        // Third argument one, bytes in chunk will be initialized to zero
        ret = chunk_hook(current_thread, n*size, 1);
        return ret;
}

void *do_heap_realloc(void *ptr, size_t size)
{
        void *ret = NULL;
        // Get thread cache for current thread
        current_thread = get_current_thread_cache(tkey);

        ret = chunk_realloc_hook(current_thread, ptr, size);
        return ret;
}
