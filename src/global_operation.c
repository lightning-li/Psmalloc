#include "global_operation.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void init_before_main(void)
{
        /* Initialize mutex tkey */
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_init(&mtx, NULL);
        pthread_key_create(&tkey, &thread_destructor);

        pthread_mutex_lock(&mutex);   // Lock
        
        /* Allocation for central and thread struct */
        thread_slab  = sbrk(thread_slab_size);
        global_add_central();
        
        pthread_mutex_unlock(&mutex); // Unlock
}

struct thread_cache *thread_init(void)
{
        struct central_cache *cc = NULL;
        struct thread_cache *tc  = NULL;

        pthread_mutex_lock(&mutex);
        
        /* Get thread cache */
        if (free_thread == NULL)
                tc = thread_slab++;
        else
                free_thread = (tc = free_thread)->next;
        
        // Check if there is free central cache
        if (free_central == NULL)
                global_add_central();
        // Get first free central cache
        free_central = (cc = free_central)->next;
        pthread_setspecific(tkey, tc);
        pthread_mutex_unlock(&mutex);
        
        central_renew(cc);
        // Initialize thread_cache_struct
        tc->count = 0;
        tc->cc = cc;
        return tc;
}

void thread_destructor(void *ptr)
{
        struct thread_cache *tc = ptr;
        if (tc->count != 0)
                check_thread_use(tc, 1);
                
        pthread_mutex_lock(&mutex);     // Lock
        tc->next = free_thread;
        free_thread = tc;
        pthread_mutex_unlock(&mutex);   // Unlock
}

void central_renew(struct central_cache *cc)
{
        int index;
        cc->next = cc;
        cc->prev = cc;
        /* Initialize first free chunk */
        cc->free_chunk = (void*)cc + chunk_size[0];
        cc->free_chunk->seek = central_cache_size - chunk_size[0];
        //        printf("renew %zu\n", cc->free_chunk->seek);
        cc->free_chunk->next = NULL;
}

void global_add_central(void)
{
        char index;
        struct central_cache *cc = NULL;

        /* Initialize first central */
        free_central = sbrk(central_cache_size);
        cc = free_central;

        /* Initialize other central */
        for (index = num_of_add_central-1; index > 0; --index) {
                cc->next = sbrk(central_cache_size);
                cc = cc->next;
        }
        cc->next = NULL;
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

        /* Initialize chunks in central */
        central_renew(new_cc);        
        /* Add new central to thread cache */
        if (tc->cc == NULL) {
                tc->cc = new_cc;
        } else {
                new_cc->prev = tc->cc->prev;
                new_cc->next = tc->cc;
                tc->cc->prev->next = new_cc;
                tc->cc->prev = new_cc;
        }
}

struct thread_cache *get_current_thread(void)
{
        struct thread_cache *tc = NULL;

        tc = pthread_getspecific(tkey);
        if (tc == NULL)
                tc = thread_init();
        
        return tc;
}

void check_thread_use(struct thread_cache *tc, int flag)
{
        struct central_cache *cc = NULL;

        if (tc->count != 0 && flag == 0)
                return;

        pthread_mutex_lock(&mutex);     // Lock

        // Add all central caches in thread to free central
        tc->cc->prev->next = free_central;
        free_central = tc->cc;
        
        pthread_mutex_unlock(&mutex);   // Unlock
        
        tc->cc = NULL;
}
