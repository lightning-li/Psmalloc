#ifndef PSMALLOC_THREAD_OPERATION_H_
#define PSMALLOC_THREAD_OPERATION_H_

#include "common.h"

void *chunk_hook(struct thread_cache *tc, size_t size, int flag);
void *chunk_realloc_hook(struct thread_cache *tc, void *ptr, size_t size);
// Find the central of a pointer
struct central_cache *find_central_of_pointer(struct thread_cache *tc,
                                              void *ptr);
static enum chunk_kind check_size(size_t size);
static void *get_suitable_chunk(struct thread_cache *tc,
                                enum chunk_kind kind,
                                int num,
                                struct chunk_head *old_ch);
static void *get_appoint_chunk(struct thread_cache *tc,
                            size_t size,
                            void *ptr);
static void add_free_chunk(struct thread_cache *tc, struct chunk_head *ch);


#endif
