/*
  **************************************************
  common.h

  Common definitions for psmalloc

  **************************************************
*/

#ifndef PSMALLOC_COMMON_H_
#define PSMALLOC_COMMON_H_

#include <stddef.h>           // for size_t
#include <stdint.h>           // for uintptr_t, uint64_t


/*
  **************************************************
  Configuration
  **************************************************
*/

static const size_t max_critical_size         = 1024*128 - 16;
static const size_t global_central_cache_size = 1024*1024*8;
static const size_t thread_central_cache_size = 1024*1024;
static const size_t thread_small_pool_size    = 64;
static const size_t thread_mediem_pool_size   = 512;
static const size_t thread_big_pool_size      = 1024*4;
static const size_t thread_huge_pool_size     = 1024*32;


/*
  **************************************************
  Define structs
  **************************************************
*/

// Struct for 


// Struct for global central cache
struct global_central_cache_struct {
        size_t index;
        uintptr_t start;
        struct global_central_cache_struct *next;
        struct global_central_cache_struct *prev;
};
