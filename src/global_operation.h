#ifndef PSMALLOC_GLOBAL_OPERATION_H_
#define PSMALLOC_GLOBAL_OPERATION_H_

#include "common.h"
#include <pthread.h>          // for pthread_key_t

/*
  variable
*/
// Pointer to allocation central_cache
static struct central_cache *central_slab = NULL;

// Pointer to allocation central_cache
static struct central_cache *thread_slab = NULL;

// List of free central_cache
static struct central_cache *free_central = NULL;

// List of free thread_cache
static struct thread_cache *free_thread = NULL;

// Key to get thread cache
static pthread_key_t tkey;
static pthread_once_t once_flag = PTHREAD_ONCE_INIT;

// Mutex when global get central cache
static pthread_mutex_t mutex;

/*
  function
*/
// For thread_once
static void once_func(void);

// Initialze for a new thread
static struct thread_cache *thread_init(void);

// When a central begin to use
static void central_renew(struct central_cache* cc);

// Add more central caches
static void global_add_central(void);

// Add a central cache to a thread
void thread_add_central(struct thread_cache *tc);

// Return current thread
struct thread_cache *get_current_thread(void);

#endif
