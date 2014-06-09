#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <psmalloc.h>
#include "common.h"

void ps_func(void)
{
        const int num = 10;
        int i = 0;
        void *p[num];
        struct chunk_head *ch = NULL;
        
        for (i=0; i<num; ++i)
                p[i] = ps_malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                p[i] = ps_realloc(p[i], i*i*100 + 400);

        for (i=0; i<num; ++i)
                ps_free(p[i]);

        pthread_exit(0);
}

void tc_func(void)
{
        const int num = 10;
        int i = 0;
        void *p[num];
        struct chunk_head *ch = NULL;
        
        for (i=0; i<num; ++i)
                p[i] = malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                p[i] = realloc(p[i], i*i*100 + 400);

        for (i=0; i<num; ++i)
                free(p[i]);

        pthread_exit(0);
}

int main(void)
{
        const int num = 32;
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
