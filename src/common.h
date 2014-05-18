/*
  **************************************************

  Common definitions for psmalloc

  **************************************************
*/

#ifndef PSMALLOC_COMMON_H_
#define PSMALLOC_COMMON_H_

#include <stddef.h>           // for size_t


/*
  **************************************************
  Configuration
  **************************************************
*/

// Size of slab for central and thread struct
static const size_t central_slab_size = 1024*500;          // 500 KB
static const size_t thread_slab_size  = 1024*50;           // 50 KB

// Size of each central cache
static const size_t central_cache_size        = 1024*1024; // 1 MB
// The number of central caches when add
static const size_t num_of_init_central = 4;

// Size of four kinds of chunk in each thread
static const size_t tiny_chunk_size    = 128;        // 128 Bytes
static const size_t small_chunk_size   = 512;        // 512 Bytes
static const size_t medium_chunk_size  = 1024*2;     // 2   KB
static const size_t big_chunk_size     = 1024*8;     // 8   KB
static const size_t huge_chunk_size    = 1024*32;    // 32  KB

enum chunk_kind {
        tiny,
        small,
        medium,
        big,
        huge
};


/*
  **************************************************
  Define structs
  **************************************************
*/

// Struct for central cache
struct central_cache {
        void                         *start;
        void                         *current;
        struct chunk_head            *free_chunk;
        struct chunk_head            *tiny_chunk;
        struct chunk_head            *small_chunk;
        struct chunk_head            *medium_chunk;
        struct chunk_head            *big_chunk;
        struct chunk_head            *huge_chunk;
        struct central_cache_struct  *next;
};

// Struct for thread cache
// Each thread has a thread_cache_struct
struct thread_cache {
        size_t                      count;
        struct central_cache_struct *cc;
        struct thread_cache_struct  *next;
};

// Each chunk has a chunk_head at the beginning
struct chunk_head {
        enum chunk_kind   kind;
        size_t            seek;
        struct chunk_head *next;
};

static const size_t chunk_head_size = sizeof(struct chunk_head);
static const size_t critical_size = huge_chunk_size - chunk_head_size;

#endif
