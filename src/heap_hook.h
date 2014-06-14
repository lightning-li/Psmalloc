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

/* Check kind and num of appointed size */
static uint8_t check_size(size_t size, uint8_t *kind);

/* Get suitable chunk */
static void *get_suitable_chunk(struct thread_cache *tc,
                                uint8_t kind,
                                uint8_t num,
                                size_t align,
                                struct chunk_head *old_ch);

/* If the appoint place is free and size is enough, then return it */
static void *get_appoint_chunk(struct central_cache *cc,
                               size_t tar_size,
                               void *ptr);

#endif
