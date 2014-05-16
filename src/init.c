#include "init.h"
#include "alloc_hook.h"
#include "free.h"
#include <unistd.h>

static void once_func(void)
{
        // mutex initialize
        pthread_mutex_init(&mutex, NULL);
        
        // Global initialize
        do_global_init();
}

static void add_central_cache(void)
{
        void *ptr = NULL;
        size_t index;
        size_t index_util;
        struct central_cache_struct *cc = NULL;

        // Initialize first central cache
        if (global_central_head == NULL) {
                index = 1;
                global_central_head = sbrk(sizeof(struct central_cache_struct));
                cc = global_central_head;
                cc->prev = NULL;
        } else {
                index = last_central->index + 1;
                last_central->next = sbrk(sizeof(struct central_cache_struct));
                cc = last_central->next;
                cc->prev = last_central;
        }
        first_free_central = cc;
        index_until = index + num_of_init_central - 1;
                
        // Initialize four central caches
        while(1) {
                cc->index = index;
                cc->tc = NULL;
                // hook central cache from system
                central_hook(cc);
                if (index == index_util)
                        break;
                cc->next = sbrk(sizeof(struct central_cache_struct));
                cc->next->prev = cc;
                cc = cc->next;
                ++index
        }
        cc->next = NULL;
        last_central = cc;
}

static struct thread_cache_struct *do_thread_init(void)
{
        struct central_cache_struct *cc = NULL;
        struct thread_cache_struct *tc  = NULL;
        pthread_mutex_lock(&mutex);

        // Check if there is free central cache
        if (first_free_central == NULL)
                add_central_cache();

        // Get thread cache for current thread
        tc = sbrk(sizeof(struct thread_cache_struct));

        // Get first free central cache
        cc = first_free_central;
        cc->tc = tc;
        pthread_setspecific(tkey, tc);
        
        // Find next free central and give it to first_free_central
        while ((first_free_central = first_free_central->next) == NULL)
                if (first_free_central->tc != NULL)
                        break;
        pthread_mutex_unlock(&mutex);

        // Initialize thread_cache_struct
        tc->alloc_count = 0;
        tc->cc = cc;
        chunk_hook(tc);

        return tc;
}

static struct thread_cache_struct *do_global_init(void)
{
        struct thread_cache_struct *tc = NULL;

        // Initialize four primary central
        add_central_struct();

        // Create thread key
        pthread_key_create(&tkey, NULL);

        // Initialize this thread cache
        tc = do_thread_init();
        return tc;
}

struct thread_cache_struct *get_current_thread_cache(void)
{
        struct thread_cache_struct *tc = NULL;

        // Initialize when the first thread come in
        pthread_once_(&once_flag, once_func);

        tc = pthread_getspecific(tkey);
        if (tc == NULL)
                tc = do_thread_init();

        return tc;
}

                        
