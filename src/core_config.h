/*
  **************************************************

  Core configuration for psmalloc

  **************************************************
*/

#ifndef PSMALLOC_CORE_CONFIG_H_
#define PSMALLOC_CORE_CONFIG_H_

#include <stddef.h>           // for size_t
#include <stdint.h>           // for uint8_t, uint16_t, uint32_t

/* Version of PSMalloc */
#define __PSMALLOC__ 0
#define __PSMALLOC_MINOR__ 1

/*
  **************************************************
  Define structs
  **************************************************
*/
/* Each thread has a thread_cache_struct */
struct thread_cache {
        struct central_cache *cc;
        struct chunk_head    *mm;
        struct thread_cache  *next;
};

/* Each central has a central_cache at the beginning */
struct central_cache {
        struct chunk_head    *free_chunk;
        struct central_cache *next;
};

/* Each chunk has a chunk_head at the beginning */
struct chunk_head {
        uint8_t           kind;     // num_of_kinds
        uint8_t           num;      // 1, 2, 3 or more
        size_t            seek;
        struct chunk_head *next;
};

/*
  **************************************************
  Configuration
  **************************************************
*/

/* Size of chunk_head */
static const size_t chunk_head_size = sizeof(struct chunk_head);

/* Size of slab for central and thread struct */
static const size_t thread_slab_size    = 100 * sizeof(struct thread_cache);

/* Size of each central cache */
static const size_t central_cache_size  = 1024*512;  // 512 KB

/* The number of central caches when add */
static const size_t num_of_add_central = 4;

/* Num of kinds */
static const uint8_t num_of_kinds = 5;

/* Size of four kinds of chunk in each thread */
static const size_t chunk_size[] = {64,          // 64 Bytes
                                    256,         // 256 Bytes
                                    1024,        // 1 KB
                                    1024*4,      // 4 KB
                                    1024*16};    // 16 KB

/* Critical size
   Allocation more than this size should use mmap */
static const size_t critical_size = 1024*512 - 1024*16;

#endif
