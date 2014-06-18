
/* 
 * Copyright (C) 2014 FillZpp
 */


#ifndef PSMALLOC_HEAP_HOOK_H_
#define PSMALLOC_HEAP_HOOK_H_


#include "core_config.h"


void *chunk_alloc_hook(size_t size, size_t align);

void *chunk_realloc_hook(void *ptr, size_t size);

void do_chunk_free(struct central_cache *cc, struct chunk_head *ch);


#endif
