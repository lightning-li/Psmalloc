#ifndef PSMALLOC_THREAD_OPERATION_H_
#define PSMALLOC_THREAD_OPERATION_H_

#include "common.h"

enum chunk_kind check_size(size_t size);
void *chunk_hook(struct thread_cache *tc, size_t size, int flag);
void *chunk_realloc_hook(struct thread_cache *tc, void *ptr, size_t size);
static void *get_suitable_chunk(struct thread_cache *tc, \
                                enum chunk_kind kind, struct chunk_head *ch);

#endif
