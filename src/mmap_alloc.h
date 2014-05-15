#ifndef PSMALLOC_MMAP_ALLOC_H_
#define PSMALLOC_MMAP_ALLOC_H_

#include <stddef.h>        // for size_t

void *do_mmap_malloc(size_t size);

void *do_mmap_calloc(size_t n, size_t size);

void *do_mmap_realloc(void *ptr, size_t size);
