/*
  **************************************************

  Common definitions for psmalloc

  **************************************************
*/

#ifndef PSMALLOC_CORE_CONF_H_
#define PSMALLOC_CORE_CONF_H_

#include <stddef.h>           // for size_t
#include <stdint.h>           // for uint32_t


/*
  **************************************************
  Configuration
  **************************************************
*/

// Size of slab for central and thread struct
static const size_t central_slab_size = 1024*250;          // 250 KB
static const size_t thread_slab_size  = 1024*50;           // 50 KB

// Size of each central cache
static const size_t central_cache_size        = 1024*1024; // 1 MB
// The number of central caches when add
static const size_t num_of_init_central = 4;

enum chunk_kind {
        tiny,
        small,
        medium,
        big,
        huge
};

// Size of four kinds of chunk in each thread
static const uint32_t chunk_size[] = {128,             // tiny chunk
                                      512,             // small chunk
                                      1024*2,          // medium chunk
                                      1024*8,          // big chunk
                                      1024*32};        // huge chunk

static const uint32_t chunk_trans[] = {1, 4, 16, 64, 256};  // when kind trans to tiny

// Max number of free chunk in a central cache
static const int max_free_chunk = 1024*1024 / 128;

/*
  **************************************************
  Define structs
  **************************************************
*/

// Struct for central cache
struct central_cache {
        void                 *start;
        void                 *end;
        struct chunk_head    *free_chunk;
        struct central_cache *next;
};

// Struct for thread cache
// Each thread has a thread_cache_struct
struct thread_cache {
        size_t               count;
        struct central_cache *cc;
        struct chunk_head    *mm;
};

// Each chunk has a chunk_head at the beginning
struct chunk_head {
        enum chunk_kind   kind;
        uint32_t          num;      // 1, 2, 3
        size_t            seek;
        struct chunk_head *next;
};

static const size_t chunk_head_size = sizeof(struct chunk_head);
static const size_t critical_size = 1024*32*3 - sizeof(struct chunk_head);

#endif
