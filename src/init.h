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
// A symbol variable
static char init_done = 0;
static pthread_key_t tkey;
// A spin lock when global get central cache from system
pthread_spinlock_t spin_lock;

/*
  function
*/
static struct thread_cache_struct *do_thread_init();
static void add_central_cache();
struct thread_cache_struct *do_global_init();
struct thread_cache_struct *get_current_thread_cache();

#endif
