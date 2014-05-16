/*
  **************************************************

  API definitions for psmalloc

  **************************************************
*/

#ifndef PSMALLOC_PSMALLOC_H_
#define PSMALLOC_PSMALLOC_H_

#include <sys/cdefs.h>        // for __THROW
#include <stddef.h>           // for size_t, NULL


void *ps_malloc(size_t size) __THROW;

void ps_free(void *ptr) __THROW;

void *ps_calloc(size_t n, size_t size) __THROW;

void *ps_realloc(void *ptr, size_t size) __THROW;

#endif
