#ifndef PSMALLOC_INIT_H_
#define PSMALLOC_INIT_H_

#include "common.h"
#include "global_operation.h"
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
// A mutex when global get central cache
static pthread_mutex_t mutex;

/*
  function
*/
// For thread_once
static void once_func(void);
static struct thread_cache *thread_init(void);
static void central_init(struct central_cache* cc);
static void global_add_central(void);
void thread_add_central(struct thread_cache *tc);
struct thread_cache *get_current_thread(void);

#endif
