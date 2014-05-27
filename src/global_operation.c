#include "global_operation.h"
#include <unistd.h>

static void once_func(void)
{
        // mutex initialize
        pthread_mutex_init(&head_mutex, NULL);
        pthread_mutex_init(&mmap_mutex, NULL);
        
        // Create thread key
        pthread_key_create(&tkey, NULL);

        pthread_mutex_lock(&heap_mutex);
        // Allocation for central and thread struct
        central_slab = sbrk(central_slab_size);
        thread_slab  = sbrk(thread_slab_size);
        pthread_mutex_unlock(&heap_mutex);
}

static struct thread_cache *thread_init(void)
{
        struct central_cache *cc = NULL;
        struct thread_cache *tc  = NULL;

        pthread_mutex_lock(&mutex);
        // Check if there is free central cache
        if (free_central == NULL)
                global_add_central();

        // Get thread cache for current thread
        if (free_thread == NULL)
                tc = thread_slab++;
        else
                free_thread = (tc = free_thread).next;

        // Get first free central cache
        free_central = (cc = free_central).next;
        pthread_setspecific(tkey, tc);
        
        pthread_mutex_unlock(&mutex);

        central_renew(cc);
        // Initialize thread_cache_struct
        tc->count = 0;
        tc->cc = cc;
        return tc;
}

static void central_renew(struct central_cache *cc)
{
        int index;
        struct chunk_head *ch;

        cc->next = NULL;
        cc->free_chunk = cc->start;
        cc->free_chunk->seek = c->start + central_cache_size;
        cc->free_chunk->next = NULL;
}

static void global_add_central(void)
{
        void *ptr = NULL;
        char index = 0;
        struct central_cache *cc = NULL;

        free_central = central_slab++;
        cc = free_central;
                
        // Initialize four central caches
        while(1) {
                cc->start = sbrk(central_cache_size);
                if (index == (num_of_init_central-1))
                        break;
                cc->next = central_slab++;
                cc = cc->next;
                ++index
        }
        cc->next = NULL;
}

void thread_add_central(struct thread_cache *tc)
{
        struct central_cache *old_cc = NULL;
        struct central_cache *new_cc = NULL;
        struct thread_cache *tc  = NULL;

        pthread_mutex_lock(&mutex);
        // Check if there is free central cache
        if (free_central == NULL)
                global_add_central();
        // Get first free central cache
        free_central = (new_cc = free_central).next;
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

        // Initialize when the first thread come in
        pthread_once(&once_flag, once_func);

        tc = pthread_getspecific(tkey);
        if (tc == NULL)
                tc = thread_init();

        return tc;
}

                        
