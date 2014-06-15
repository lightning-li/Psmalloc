#include "global_operation.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/* Pointer to allocate central_cache */
static struct thread_cache *thread_slab = NULL;

/* List of free thread_cache */
static struct thread_cache *free_thread = NULL;

/* List of free central_cache */
static struct central_cache *free_central = NULL;

/* Key to get thread cache */
static pthread_key_t tkey;

/* Mutex when global get central cache */
static pthread_mutex_t mutex;

void global_add_central(void)
{
        int index;
        struct central_cache *cc = NULL;

        /* Initialize first central cahce */
        free_central = sbrk(central_cache_size);
        cc = free_central;

        /* Initialize other central cache */
        for (index = num_of_add_central-1; index > 0; --index) {
                cc->next = sbrk(central_cache_size);
                cc = cc->next;
        }
        cc->next = NULL;
}

void thread_destructor(void *ptr)
{
        struct thread_cache *tc = ptr;
        struct central_cache *cc = tc->cc;
        
        while (cc->next != NULL)
                cc = cc->next;

        pthread_mutex_lock(&mutex);     // Lock

        /* Give back all central caches in thread */
        cc->next = free_central;
        free_central = tc->cc;
        /* Give back tc */
        tc->next = free_thread;
        free_thread = tc;

        pthread_mutex_unlock(&mutex);   // Unlock
}

struct thread_cache *thread_init(void)
{
        struct central_cache *cc = NULL;
        struct thread_cache *tc  = NULL;

        pthread_mutex_lock(&mutex);     // Lock
        
        /* Get struct thread_cache */
        if (free_thread == NULL)
                tc = thread_slab++;
        else
                free_thread = (tc = free_thread)->next;
        
        /* Check if there is free central cache */
        if (free_central == NULL)
                global_add_central();
        /* Get a free central */
        free_central = (cc = free_central)->next;
        /* Set tc as thread private data */
        pthread_setspecific(tkey, tc);
        
        pthread_mutex_unlock(&mutex);   // Unlock
        
        central_renew(cc);
        tc->cc = cc;
        tc->mm = NULL;
        
        return tc;
}

void init_before_main(void)
{
        /* Initialize mutex, tkey */
        pthread_mutex_init(&mutex, NULL);
        pthread_key_create(&tkey, &thread_destructor);
        /* Allocate for struct thread_cache */
        thread_slab  = sbrk(thread_slab_size);

        /* Initialize global central cache
           and allocate for main thread*/
        global_add_central();
        thread_init();
}

void central_renew(struct central_cache *cc)
{
        cc->next = NULL;
        /* Initialize first free chunk */
        cc->free_chunk = (void*)cc + chunk_size[0];
        cc->free_chunk->seek = central_cache_size - chunk_size[0];
        cc->free_chunk->next = NULL;
}

void thread_add_central(struct thread_cache *tc)
{
        struct central_cache *new_cc = NULL;

        pthread_mutex_lock(&mutex);     // Lock
        
        /* Check if there is free central cache */
        if (free_central == NULL)
                global_add_central();
        
        /* Get first free central cache */
        free_central = (new_cc = free_central)->next;
        
        pthread_mutex_unlock(&mutex);   // Unlock

        central_renew(new_cc);
        new_cc->next = tc->cc;
        tc->cc = new_cc;
}

struct thread_cache *get_current_thread(void)
{
        struct thread_cache *tc = pthread_getspecific(tkey);
        
        if (tc == NULL)
                tc = thread_init();
        
        return tc;
}

