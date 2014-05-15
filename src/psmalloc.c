#include "psmalloc.h"
#include "free.h"
#include "common.h"
#include "heap_alloc.h"
#include "mmap_alloc.h"

void *ps_malloc(size_t size) __THROW
{
        void *ret = NULL;
        if (size > max_critical_size) {
                ret = do_mmap_malloc(size);
        } else {
                ret = do_heap_malloc(size);
        }
        return ret;
}

void *ps_calloc(size_t n, size_t size) __THROW
{
        void *ret = NULL;
        if (n*size > max_critical_size) {
                ret = do_mmap_calloc(n, size);
        } else {
                ret = do_heap_calloc(n, size);
        }
        return ret;
}

void *ps_realloc(void *ptr, size_t size) __THROW
{
        if (size > max_critical_size) {
                ret = do_mmap_realloc(ptr, size);
        } else {
                ret = do_heap_realloc(ptr, size);
        }
        return ret;
}

void ps_free(void *ptr) __THROW
{
        do_free(void *ptr);
}
