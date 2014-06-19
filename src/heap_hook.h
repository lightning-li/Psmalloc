
/* 
 * Copyright (C) 2014 FillZpp
 */


#ifndef _PSMALLOC_HEAP_HOOK_H
#define _PSMALLOC_HEAP_HOOK_H


#include "core_config.h"

/* Allocate chunk in central cache */
void *chunk_alloc_hook(size_t size, size_t align);

/* Reallocate chunk in central cache */
void *chunk_realloc_hook(void *ptr, size_t size);

/* Release chunk in central cache */
void do_chunk_free(struct central_cache *cc, struct chunk_head *ch);


#endif
