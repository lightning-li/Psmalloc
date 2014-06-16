
/*
 * Copyright (C) 2014 FillZpp
 */


#ifndef PSMALLOC_HEAP_HOOK_H_
#define PSMALLOC_HEAP_HOOK_H_


#include "core_config.h"


void *chunk_alloc_hook(struct thread_cache *tc,
                       size_t size, size_t align, int flag);

void *chunk_realloc_hook(struct thread_cache *tc, void *ptr, size_t size);

void do_chunk_free(struct central_cache *cc, struct chunk_head *ch);

/* Find the central of a pointer */
struct central_cache *find_central_of_pointer(struct thread_cache *tc,
                                              void *ptr);


#endif
