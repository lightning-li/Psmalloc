#ifndef PSMALLOC_ALLOC_H_
#define PSMALLOC_ALLOC_H_

#include <stddef.h>         // for size_t

void *do_malloc(size_t size) __THROW;

void *do_calloc(size_t n, size_t size) __THROW;

void *do_realloc(void *ptr, size_t size) __THROW;

