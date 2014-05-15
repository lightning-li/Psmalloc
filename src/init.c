#include "init.h"
#include "alloc_hook.h"
#include <pthread.h>
#include <unistd.h>

struct thread_cache_struct *has_init()
{
        struct central_cache_struct *cc = global_central_head;
        struct thread_cache_struct  *tc = NULL;
        pthread_t tid                   = pthread_self();

        // Check if this process has been initialized
        if (cc == NULL)
                return NULL;

        
        // Check if this thread has been initialized
        while (cc != NULL && (tc = cc->tc) != NULL) {
                if (tid == tc->tid)
                        break;
                else
                        tc = NULL;
                cc = cc->next;
        }
        return tc;
}

struct thread_cache_struct *do_init()
{
        struct central_cache_struct *cc = NULL;
        pthread_t tid                   = pthread_self();
        int index;

        /*
          Process initialize
        */
        if (global_central_head == NULL) {
                global_central_head = sbrk(sizeof(struct central_cache_struct));
                global_central_head->prev = NULL;
                cc = global_central_head;
                
                // Initialize four central caches
                index = 0;
                while(1) {
                        cc->index = index;
                        cc->tc    = NULL;
                        central_hook(cc);
                        if (index == 3)
                                break;
                        cc->next = sbrk(sizeof(struct central_cache_struct));
                        cc->next->prev = cc;
                        cc = cc->next;
                }
                cc->next = NULL;
                first_free_central = global_central_head;
                last_central = cc;
        }

        /*
          Thread initialize
        */
        // If there is no free central cache
        if (first_free_central == NULL)
                
                
        
