#ifndef PSMALLOC_MMAP_HOOK_H_
#define PSMALLOC_MMAP_HOOK_H_

#include <stddef.h>        // for size_t
#include "common.h"

void *mmap_alloc_hook(struct thread_cache *tc, size_t size, int flag);
void *mmap_realloc_hook(struct thread_cache *tc, void *ptr, size_t size);
void do_mmap_free(struct thread_cache *tc, struct chunk_head *old_mm);


#endif
