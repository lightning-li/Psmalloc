#ifndef PSMALLOC_INIT_H_
#define PSMALLOC_INIT_H_

#include "common.h"

// Head of the central_cache_struct list
static struct central_cache_struct *global_central_head;

// Symbol variable to check if the global central has been initialized
// One or zero
static char global_done;

// Check if it has initialized in this process or this thread
struct thread_cache_struct *has_init();

// Initialize for new process or new thread
struct thread_cache_struct *do_init();
