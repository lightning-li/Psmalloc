#include "global_operation.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void init_before_main(void)
{
        // mutex initialize
        pthread_mutex_init(&mutex, NULL);
        // Create thread key
        pthread_key_create(&tkey, NULL);

        pthread_mutex_lock(&mutex);
        // Allocation for central and thread struct
        central_slab = sbrk(central_slab_size);
        thread_slab  = sbrk(thread_slab_size);
        global_add_central();
        pthread_mutex_unlock(&mutex);
}

struct thread_cache *thread_init(void)
{
        struct central_cache *cc = NULL;
        struct thread_cache *tc  = NULL;

        pthread_mutex_lock(&mutex);
        // Get thread cache
        tc = thread_slab++;
        
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

void central_renew(struct central_cache *cc)
{
        int index;

        cc->next = NULL;
        cc->free_chunk = cc->start;
        cc->free_chunk->seek = cc->end;
        cc->free_chunk->num = max_free_chunk;
        cc->free_chunk->next = NULL;
}

void global_add_central(void)
{
        char index = 1;
        struct central_cache *cc = NULL;

        free_central = central_slab++;
        cc = free_central;

        // Initialize four central caches
        while(1) {
                cc->start = sbrk(central_cache_size);
                cc->end   = cc->start + central_cache_size -1;
                if (index == num_of_init_central)
                        break;
                cc->next = central_slab++;
                cc = cc->next;
                ++index;
        }
        cc->next = NULL;
}

void thread_add_central(struct thread_cache *tc)
{
        struct central_cache *old_cc = NULL;
        struct central_cache *new_cc = NULL;

        pthread_mutex_lock(&mutex);
        // Check if there is free central cache
        if (free_central == NULL)
                global_add_central();
        // Get first free central cache
        free_central = (new_cc = free_central)->next;
        pthread_mutex_unlock(&mutex);

        // Initialize chunks in central
        central_renew(new_cc);
        // Add new central to thread cache
        if ((old_cc = tc->cc) == NULL) {
                tc->cc = new_cc;
        } else {
                while (old_cc->next != NULL)
                        old_cc = old_cc->next;
                old_cc->next = new_cc;
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

void check_thread_use(struct thread_cache *tc)
{
        struct central_cache *cc = NULL;

        if (tc->count != 0)
                return;

        pthread_mutex_lock(&mutex);
        // Add all central caches in thread to free central
        cc = tc->cc;
        while (cc->next!=NULL)
                cc = cc->next;
        cc->next = free_central;
        free_central = tc->cc;
        pthread_mutex_unlock(&mutex);

        tc->cc = NULL;
}
