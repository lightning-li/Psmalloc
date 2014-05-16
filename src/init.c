#include "init.h"
#include "alloc_hook.h"
#include "free.h"
#include <unistd.h>

static void add_central_cache()
{
        size_t index                       = last_central->index + 1;
        size_t index_util                  = index + 3;
        struct central_cache_struct *cc = NULL;

        // Initialize first central cache
        if (global_central_head == NULL) {
                global_central_head = sbrk(sizeof(struct central_cache_struct));
                cc = global_central_head;
                cc->prev = NULL;
        } else {
                last_central->next = sbrk(sizeof(struct central_cache_struct));
                cc = last_central->next;
                cc->prev = last_central;
        }
        first_free_central = cc;
                
        // Initialize next three central caches
        while(1) {
                cc->index = index;
                cc->tc    = NULL;
                central_hook(cc);
                if (index == index_util)
                        break;
                cc->next = sbrk(sizeof(struct central_cache_struct));
                cc->next->prev = cc;
                cc = cc->next;
        }
        cc->next = NULL;
        last_central = cc;
}

static struct thread_cache_struct *do_thread_init()
{
        
}

struct thread_cache_struct *do_global_init()
{
        struct thread_cache_struct *tc = NULL;

        init_done = 1;
        // Initialize four primary central
        add_central_struct();

        // Create thread key
        pthread_key_create(&tkey, NULL);

        // Initialize this thread cache
        tc = do_thread_init();
        return tc;
}

struct thread_cache_struct *get_current_thread_cache()
{
        struct thread_cache_struct *tc = NULL;

        tc = pthread_getspecific(tkey);
        if (tc == NULL)
                tc = do_thread_init();

        return tc;
}

                        
