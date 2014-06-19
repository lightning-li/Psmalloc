
/* 
 * Copyright (C) FillZpp
 */


#ifndef _PSMALLOC_MMAP_HOOK_H
#define _PSMALLOC_MMAP_HOOK_H


#include "core_config.h"
#include <stddef.h>        // for size_t


void *mmap_alloc_hook(size_t size);

void *mmap_realloc_hook(void *ptr, size_t size);

void do_mmap_free(struct chunk_head *old_mm);


#endif
