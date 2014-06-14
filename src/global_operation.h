#ifndef PSMALLOC_GLOBAL_OPERATION_H_
#define PSMALLOC_GLOBAL_OPERATION_H_

#include "core_config.h"


/* Global initialize before main function */
static void init_before_main(void) __attribute__((constructor));

/* Initialze for a new thread */
static struct thread_cache *thread_init(void);

/* Destruct when thread exit */
static void thread_destructor(void *ptr);

/* Renew a central before use */
static void central_renew(struct central_cache* cc);

/* Add more central caches for global */
static void global_add_central(void);

/* Add a central cache for a thread */
void thread_add_central(struct thread_cache *tc);

/* Return current thread */
struct thread_cache *get_current_thread(void);


#endif
