#ifndef PSMALLOC_INIT_H_
#define PSMALLOC_INIT_H_

#include "common.h"
#include <pthread.h>          // for pthread_key_t

/*
  variable
*/
// Head of the central_cache_struct list
static struct central_cache_struct *global_central_head = NULL;
// First free central cache
static struct central_cache_strcut *first_free_central = NULL;
// Last central cache
static struct central_cache_struct *last_central = NULL;
// Key to get thread cache
static pthread_key_t tkey;
static pthread_once_t once_flag = PTHREAD_ONCE_INIT;
// A mutex when global get central cache
static pthread_mutex_t mutex;

/*
  function
*/
static void once_func(void);
static void add_central_cache(void);
static struct thread_cache_struct *do_thread_init(void);
static struct thread_cache_struct *do_global_init(void);
struct thread_cache_struct *get_current_thread_cache(void);

#endif
