/*
  **************************************************

  Common definitions for psmalloc

  **************************************************
*/

#ifndef PSMALLOC_COMMON_H_
#define PSMALLOC_COMMON_H_

#include <stddef.h>           // for size_t
#include <stdint.h>           // for uintptr_t, uint64_t
#include <pthread.h>          // for pthread_t


/*
  **************************************************
  Configuration
  **************************************************
*/

// If the size of once allocation is more than this size
// then use mmap
static const size_t max_critical_size         = 1024*128;  // 128 KB

// Size of each central cache
static const size_t central_cache_size        = 1024*1024; // 1 MB

// The number of central caches when initialize
static const size_t num_of_init_central = 4;

// Size of four kinds of chunk in each thread
static const size_t thread_small_chunk_size   = 64;         // 64  Bytes
static const size_t thread_medium_chunk_size  = 512;        // 512 Bytes
static const size_t thread_big_chunk_size     = 1024*4;     // 4   KB
static const size_t thread_huge_chunk_size    = 1024*32;    // 32  KB

enum chunk_kind {
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
// Size of each cache is thread_central_cache_size
struct central_cache_struct {
        size_t                       index;
        void                         *start;
        struct thread_central_struct *tc;
        struct central_cache_struct  *next;
        struct central_cache_struct  *prev;
};

// Struct for thread cache
// Each thread has a thread_cache_struct
struct thread_cache_struct {
        size_t                      alloc_count;
        struct central_cache_struct *cc;
        enum chunk_kind             *free_ptr;
        enum chunk_kind             *small_ptr;
        enum chunk_kind             *medium_ptr;
        enum chunk_kind             *big_ptr;
        enum chunk_kind             *huge_ptr;
};

#endif
