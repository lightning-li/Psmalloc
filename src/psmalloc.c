/*
  **************************************************

  Realize API

  **************************************************
*/

#include "psmalloc.h"
#include "alloc.h"
#include "free.h"


void *ps_malloc(size_t size)
{
        void *result = do_malloc(size);
        return result;
}

void ps_free(void *ptr)
{
        do_free(ptr);
}

void *ps_calloc(size_t n, size_t size)
{
        void *result = do_calloc(n, size);
        return result;
}
        
void *ps_realloc(void *ptr, size_t size)
{
        void *result = do_realloc(ptr, size);
        return result;
}
