#ifndef PSMALLOC_THREAD_OPERATION_H_
#define PSMALLOC_THREAD_OPERATION_H_

#include "common.h"

void *chunk_alloc_hook(struct thread_cache *tc, size_t size, int flag);
void *chunk_realloc_hook(struct thread_cache *tc, void *ptr, size_t size);
// Find the central of a pointer
struct central_cache *find_central_of_pointer(struct thread_cache *tc,
                                              void *ptr);
// Add a chunk to free_chunk
void add_free_chunk(struct central_cache *cc, struct chunk_head *ch);
static int check_size(size_t size, enum chunk_kind &kind);
static void *get_suitable_chunk(struct thread_cache *tc,
                                enum chunk_kind kind,
                                int num,
                                struct chunk_head *old_ch);
static void *get_appoint_chunk(struct central_cache *cc,
                            size_t size,
                            void *ptr);

#endif
