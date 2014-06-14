#include "psmalloc.h"
#include "core_config.h"
#include "global_operation.h"
#include "heap_hook.h"
#include "mmap_hook.h"
#include "libc_override.h"
#include <unistd.h>           // For getpagesize()

void *ps_malloc(size_t size)
{
        void *ret = do_malloc(size, 0, 0);
        return ret;
}

void *ps_calloc(size_t n, size_t size)
{
        void *ret = do_malloc(n*size, 0, 1);
        return ret;
}

void ps_free (void *ptr)
{
        do_free(ptr);
}

void ps_cfree(void *ptr)
{
        do_free(ptr);
}

void *ps_memalign(size_t align, size_t size)
{
        void *ret = do_malloc(size, align, 0);
        return ret;
}

void *ps_valloc(size_t size)
{
        void *ret = ps_memalign(getpagesize(), size);
        return ret;
}

void *ps_realloc(void *ptr, size_t size)
{
        void *ret = NULL;
        struct thread_cache *current_thread = NULL;

        /* Check pointer */
        if (ptr == NULL) {
                ret = do_malloc(size, 0, 0);
                return ret;
        }
        /* Check size */
        if (size == 0) {
                do_free(ptr);
                return NULL;
        }

        current_thread = get_current_thread();
        if (size < critical_size) {
                ret = chunk_realloc_hook(current_thread, ptr, size);
        } else {
                ret = mmap_realloc_hook(current_thread, ptr, size);
        }
        return ret;
}

void *do_malloc(size_t size, size_t align, int flag)
{
        void *ret = NULL;
        struct thread_cache *current_thread = NULL;

        /* Check size */
        if (size == 0)
                return NULL;

        current_thread = get_current_thread();
        if (size < critical_size) {
                ret = chunk_alloc_hook(current_thread, size, align, flag);
        } else {
                ret = mmap_alloc_hook(current_thread, size, flag);
        }

        return ret;
}

void do_free(void *ptr)
{
        struct central_cache *cc = NULL;
        struct thread_cache *current_thread = NULL;

        /* Check pointer */
        if (ptr == NULL)
                return;

        current_thread = get_current_thread();
        cc = find_central_of_pointer(current_thread, ptr);
        if (cc != NULL)
                do_chunk_free(cc, ptr - chunk_head_size);
        else 
                do_mmap_free(current_thread, ptr - chunk_head_size);
}
