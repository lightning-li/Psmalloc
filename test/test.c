#include <stdio.h>
#include <pthread.h>
#include <psmalloc.h>
#include "common.h"

void thread_func(void)
{
        struct chunk_head *ch = NULL;
        void *p = ps_malloc(200);
        printf("thread: %zu\n", (p - chunk_head_size));
}

int main(void)
{
        const int num = 4;
        int i = 0;
        pthread_t tid[num];

        for (; i<num; ++i) {
                pthread_create(&tid[i], NULL, (void*)thread_func, NULL);
        }

        for (i=0; i<num; ++i)
                pthread_join(tid[i], NULL);
        
        return 0;
}
