#ifndef PSMALLOC_LIBC_OVERRIDE_H_
#define PSMALLOC_LIBC_OVERRIDE_H_

#include "psmalloc.h"
#include <sys/cdefs.h>        // for __THROW

#ifndef __THROW
#define __THROW
#endif

#define ALIAS(tc_fn)   __attribute__ ((alias (#tc_fn)))

/* override */
void *malloc(size_t size) __THROW                 ALIAS(ps_malloc);
void free(void *ptr) __THROW                      ALIAS(ps_free);
void *calloc(size_t n, size_t size) __THROW       ALIAS(ps_calloc);
void *realloc(void *ptr, size_t size) __THROW     ALIAS(ps_realloc);


#endif

