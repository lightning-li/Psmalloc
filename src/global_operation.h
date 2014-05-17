#ifndef PSMALLOC_INIT_H_
#define PSMALLOC_INIT_H_

#include "common.h"
#include "global_operation.h"
#include <pthread.h>          // for pthread_key_t

/*
  variable
*/
// Pointer to allocation central_cache_struct
static struct central_cache_struct *central_slab = NULL;
// Pointer to allocation central_cache_struct
static struct central_cache_struct *thread_slab = NULL;
// List of free central_cache_struct
static struct central_cache_strcut *free_central = NULL;
// List of free thread_cache_struct
static struct thread_cache_struct *free_thread = NULL;
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
static struct thread_cache_struct *thread_init(void);
void add_central(void);
struct thread_cache_struct *get_current_thread_cache(void);

#endif
