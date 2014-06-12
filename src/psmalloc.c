#include "psmalloc.h"
#include "core_config.h"
#include "global_operation.h"
#include "heap_hook.h"
#include "mmap_hook.h"
//#include "libc_override.h"
#include <stdio.h>
#include <stdlib.h>

void *ps_malloc(size_t size)
{
        void *ret = NULL;
        struct thread_cache *current_thread = get_current_thread();
        
        if (size < critical_size) {
                // Third argument is zero
                // Bytes in chunk will not be initialized 
                ret = chunk_alloc_hook(current_thread, size, 0);
        } else {
                ret = mmap_alloc_hook(current_thread, size, 0);
        }
        current_thread->count++;
        return ret;
}

void *ps_calloc(size_t n, size_t size)
{
        void *ret = NULL;
        struct thread_cache *current_thread = get_current_thread();

        if (n*size < critical_size) {
                // Third argument is one
                // Bytes in chunk will be initialized
                ret = chunk_alloc_hook(current_thread, size*n, 1);
        } else {
                ret = mmap_alloc_hook(current_thread, size*n, 1);
        }
        current_thread->count++;
        return ret;
}

void *ps_realloc(void *ptr, size_t size)
{
        void *ret = NULL;
        struct thread_cache *current_thread = get_current_thread();

        if (size < critical_size) {
                ret = chunk_realloc_hook(current_thread, ptr, size);
        } else {
                ret = mmap_realloc_hook(current_thread, ptr, size);
        }
        return ret;
}

void ps_free(void *ptr)
{
        struct central_cache *cc = NULL;
        struct thread_cache *current_thread = get_current_thread();
        //        printf("ps_free   middle: %zu\n", current_thread->count);        
        cc = find_central_of_pointer(current_thread, ptr);

        //        printf("ps_free   middle  sec\n");
        if (cc != NULL)
                do_chunk_free(cc, ptr - chunk_head_size);
        else
                do_mmap_free(current_thread, ptr - chunk_head_size);

        //        printf("ps_free   middle  thi\n");
        current_thread->count--;

        // Check if this thread till be used
        check_thread_use(current_thread, 0);
        //        printf("ps_free   end\n");
}
