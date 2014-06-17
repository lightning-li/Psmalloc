
/*
 * Copyright (C) 2014 FillZpp
 */


#ifndef PSMALLOC_GLOBAL_OPERATION_H_
#define PSMALLOC_GLOBAL_OPERATION_H_


#include "core_config.h"


/* Global initialize before main function */
static void init_before_main(void) __attribute__((constructor));

/* Add a central cache for a thread */
void thread_add_central(struct thread_cache *tc);

/* Return current thread */
struct thread_cache *get_current_thread(void);

/* Find out which central is a pointer in */
struct central_cache *find_central_of_pointer(void *ptr);


#endif
