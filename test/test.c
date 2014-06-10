#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include <psmalloc.h>
#include "common.h"

void tc_func(void)
{
        const int num = 30;
        int i = 0;
        void *p1[num];
        void *p2[num];
        struct chunk_head *ch = NULL;
        
        for (i=0; i<num; ++i)
                p1[i] = malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                p1[i] = realloc(p1[i], i*i*100 + 400);

        for (i=0; i<num; ++i)
                p2[i] = malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                free(p1[i]);

        for (i=0; i<num; ++i)
                p2[i] = realloc(p2[i], i*i*100 + 400);

        for (i=0; i<num; ++i)
                free(p2[i]);

        pthread_exit(0);
}

void ps_func(void)
{
        const int num = 30;
        int i = 0;
        void *p1[num];
        void *p2[num];
        struct chunk_head *ch = NULL;

        for (i=0; i<num; ++i)
                p1[i] = ps_malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                p1[i] = ps_realloc(p1[i], i*i*100 + 400);

        for (i=0; i<num; ++i)
                p2[i] = ps_malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                ps_free(p1[i]);
        
        for (i=0; i<num; ++i)
                p2[i] = ps_realloc(p2[i], i*i*100 + 400);
        
        for (i=0; i<num; ++i)
                ps_free(p2[i]);

}

int main(void)
{
        const int num = 10;
        int i = 0;
        pthread_t tid[num];
        time_t timer1, timer2;
        clock_t c1, c2;

        c1 = clock();
        for (; i<num; ++i) {
                pthread_create(&tid[i], NULL, (void*)ps_func, NULL);
        }

        for (i=0; i<num; ++i)
                pthread_join(tid[i], NULL);
        c2 = clock();
        printf("time: %e\n", (double)(c2-c1));
        
        return 0;
}
