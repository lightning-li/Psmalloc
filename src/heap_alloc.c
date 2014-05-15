#include "common.h"
#include "heap_alloc.h"
#include "init.h"

void *do_heap_malloc(size_t size)
{
        void *ret = NULL;
        struct thread_cache_struct *current_thread = NULL;

        /*
          If it is the first alloc in this process or this new thread,
          has_init() returns NULL,
          then use do_init() to initialize
        */
        if ((current_thread = has_init()) == NULL) {
                current_thread = do_init();
                if (current_thread == NULL)
                        return ret;
        }

        // Get new chunk in current thread cache
        // Third argument is zero, bytes will not be initialized to zero
        ret = chunk_hook(current_thread, size, 0);
        return ret;
}

void *do_heap_calloc(size_t n, size_t size)
{
        void *ret = NULL;
        struct thread_cache_struct *current_thread = NULL;

        /*
          If it is the first alloc in this process or this new thread,
          has_init() returns NULL,
          then use do_init() to initialize
        */
        if ((current_thread = has_init()) == NULL) {
                current_thread = do_init();
                if (current_thread == NULL)
                        return ret;
        }

        // Get new chunk in current thread cache
        // Third argument is one, bytes will be initialized to zero
        ret = chunk_hook(current_thread, n*size, 1);
        return ret;
}

void *do_heap_realloc(void *ptr, size_t size)
{
        void *ret = NULL;
        struct thread_cache_struct *current_thread = has_init();

        ret = chunk_realloc_hook(current_thread, ptr, size);
        return ret;
}
