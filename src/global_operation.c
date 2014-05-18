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

static struct thread_cache *thread_init(void)
{
        struct central_cache *cc = NULL;
        struct thread_cache *tc  = NULL;

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

        // Initialize chunks in central
        central_init(cc);
        // Initialize thread_cache_struct
        tc->count = 0;
        tc->cc = cc;
        return tc;
}

void central_init(struct central_cache *cc)
{
        int index;
        struct chunk_head *ch;

        cc->tiny_chunk = NULL;
        cc->small_chunk = NULL;
        cc->medium_chunk = NULL;
        cc->big_chunk = NULL;
        cc->huge_chunk = NULL;
        cc->next = NULL;
        
        // Renew current
        cc->current = cc->start;
        cc->free_chunk = cc->current;

        // For 4 tiny chunks
        for (index=0; index<4; ++index) {
                ch = cc->current;
                cc->current += tiny_chunk_size;
                ch->kind = tiny;
                ch->next = cc->current;
        }
        // For 4 small chunks
        for (index=0; index<4; ++index) {
                ch = cc->current;
                cc->current += small_chunk_size;
                ch->kind = small;
                ch->next = cc->current;
        }
        // For 4 medium chunks
        for (index=0; index<4; ++index) {
                ch = cc->current;
                cc->current += medium_chunk_size;
                ch->kind = medium;
                ch->next = cc->current;
        }
        // For 4 big chunks
        for (index=0; index<4; ++index) {
                ch = cc->current;
                cc->current += big_chunk_size;
                ch->kind = big;
                ch->next = cc->current;
        }
        // For 1 huge chunk
        ch = cc->current;
        cc->current += huge_chunk_size;
        ch->kind = huge;
        ch->next = NULL;
}

void add_central(void)
{
        void *ptr = NULL;
        char index = 0;
        struct central_cache *cc = NULL;

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

                        
