#include "init.h"
#include "alloc_hook.h"
#include "free.h"
#include <unistd.h>

static void once_func(void)
{
        // mutex initialize
        pthread_mutex_init(&mutex, NULL);

        // Create thread key
        pthread_key_create(&tkey, NULL);

        pthread_mutex_lock(&mutex);
        // Allocation for central and thread struct
        central_slab = sbrk(central_slab_size);
        thread_slab  = sbrk(thread_slab_size);
        pthread_mutex_unlock(&mutex);
}

static void add_central(void)
{
        void *ptr = NULL;
        char index = 0;
        struct central_cache_struct *cc = NULL;

        free_central = central_slab++;
        cc = free_central;
                
        // Initialize four central caches
        while(1) {
                cc->start = sbrk(central_cache_size);
                if (index == 3)
                        break;
                cc->next = central_slab++;
                cc = cc->next;
                ++index
        }
        cc->next = NULL;
}

static struct thread_cache_struct *thread_init(void)
{
        struct central_cache_struct *cc = NULL;
        struct thread_cache_struct *tc  = NULL;

        pthread_mutex_lock(&mutex);
        // Check if there is free central cache
        if (free_central == NULL)
                add_central();

        // Get thread cache for current thread
        if (free_thread == NULL)
                tc = thread_slab++;
        else
                free_thread = (tc = free_thread).next;

        // Get first free central cache
        free_central = (cc = free_central).next;
        pthread_setspecific(tkey, tc);
        
        pthread_mutex_unlock(&mutex);

        // Initialize central_cache_struct
        cc->free_ptr   = cc->start;
        cc->small_ptr  = NULL;
        cc->medium_ptr = NULL;
        cc->big_ptr    = NULL;
        cc->huge_ptr   = NULL;
        cc->next       = NULL;
        // Initialize thread_cache_struct
        tc->alloc_count = 0;
        tc->cc = cc;
        return tc;
}

struct thread_cache_struct *get_current_thread_cache(void)
{
        struct thread_cache_struct *tc = NULL;

        // Initialize when the first thread come in
        pthread_once(&once_flag, once_func);

        tc = pthread_getspecific(tkey);
        if (tc == NULL)
                tc = thread_init();

        return tc;
}

                        
