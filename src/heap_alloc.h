#ifndef PSMALLOC_HEAP_ALLOC_H_
#define PSMALLOC_HEAP_ALLOC_H_

#include <stddef.h>          // for size_t

void *do_heap_malloc(size_t size);
void *do_heap_calloc(size_t n, size_t size);
void *do_heap_realloc(void *ptr, size_t size);
void do_free(void *ptr);

#endif
