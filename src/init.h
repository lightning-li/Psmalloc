#ifndef PSMALLOC_INIT_H_
#define PSMALLOC_INIT_H_

#include "common.h"

// Head of the central_cache_struct list
static struct central_cache_struct *global_central_head = NULL;

// First free central cache
static struct central_cache_strcut *first_free_central = NULL;

// last central cache
static struct central_cache_struct *last_central = NULL;

// Check if it has initialized in this process or this thread
struct thread_cache_struct *has_init();

// Initialize for new process or new thread
struct thread_cache_struct *do_init();

#endif
