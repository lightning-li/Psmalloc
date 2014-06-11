#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include <psmalloc.h>
#include "core_config.h"

void func(void)
{
        const int num = 20;
        int i = 0;
        void *p1[num];
        void *p2[num];
        struct chunk_head *ch = NULL;
        
        for (i=0; i<num; ++i)
                p1[i] = malloc(i*i*100 + 100);

        printf("111\n");
        for (i=0; i<num; ++i)
                p1[i] = realloc(p1[i], i*i*100 + 400);

        printf("222\n");
        for (i=0; i<num; ++i)
                p2[i] = malloc(i*i*100 + 100);

        printf("333\n");
        for (i=0; i<num; ++i)
                free(p1[i]);

        printf("444\n");
        for (i=0; i<num; ++i)
                p2[i] = realloc(p2[i], i*i*100 + 400);

        printf("555\n");
        for (i=0; i<num; ++i)
                free(p2[i]);

        printf("666\n");
        pthread_exit(0);
}

int main(void)
{
        const int num = 1;
        int i = 0;
        pthread_t tid[num];
        time_t timer1, timer2;
        clock_t c1, c2;

        c1 = clock();
        for (; i<num; ++i) {
                pthread_create(&tid[i], NULL, (void*)func, NULL);
        }
        
        for (i=0; i<num; ++i)
                pthread_join(tid[i], NULL);
        c2 = clock();
        printf("time: %e\n", (double)(c2-c1));
        printf("sbrk: %p\n", sbrk(0));

        
        return 0;
}
