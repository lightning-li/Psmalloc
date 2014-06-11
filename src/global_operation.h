#ifndef PSMALLOC_GLOBAL_OPERATION_H_
#define PSMALLOC_GLOBAL_OPERATION_H_

#include "core_config.h"
#include <pthread.h>          // for pthread_key_t

/*
  variable
*/
// Pointer to allocation central_cache
static struct thread_cache *thread_slab = NULL;

// List of free thread_cache
static struct thread_cache *free_thread = NULL;

// List of free central_cache
static struct central_cache *free_central = NULL;

// Key to get thread cache
static pthread_key_t tkey;

// Mutex when global get central cache
static pthread_mutex_t mutex;

/*
  function
*/
// Global initialize
static void init_before_main(void) __attribute__((constructor));

// Initialze for a new thread
static struct thread_cache *thread_init(void);

// When thread exit
static void thread_destructor(struct thread_cache *tc);

// When a central begin to use
static void central_renew(struct central_cache* cc);

// Add more central caches
static void global_add_central(void);

// Add a central cache to a thread
void thread_add_central(struct thread_cache *tc);

// Return current thread
struct thread_cache *get_current_thread(void);

void check_thread_use(struct thread_cache *tc, int flag);


#endif
