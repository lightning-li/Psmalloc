#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include "core_config.h"

void func(void)
{
        const int num = 10;
        int i = 0;
        void *p1[num];
        void *p2[num];
        void *p;
        struct chunk_head *ch = NULL;

        for (i=0; i<num; ++i)
                p1[i] = malloc(i*i*100 + 400);
        p = p1[0];

        for (i=0; i<num; ++i)
                p1[i] = realloc(p1[i], i*i*100 + 100);

        for (i=0; i<num; ++i)
                p2[i] = malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                free(p1[i]);

        for (i=0; i<num; ++i)
                p2[i] = realloc(p2[i], i*i*100 + 400);
        for (i=0; i<num; ++i)
                free(p2[i]);

        ch = p - chunk_head_size;
        //printf("%p , %d, %d, %zu, %p\n", ch, ch->kind, ch->num, ch->seek, ch->next);
        pthread_exit(0);
}

int main(void)
{
        const int num = 1000;
        int i = 0;
        pthread_t tid[num];
        void *p = NULL;
        clock_t c1, c2;

        c1 = clock();
        for (; i<num; ++i) {
                pthread_create(&tid[i], NULL, (void*)func, NULL);
        }
        
        for (i=0; i<num; ++i)
                pthread_join(tid[i], NULL);
        c2 = clock();
        p = malloc(100);
        printf("std:\n");
        printf("time: %lf\n", (double)(c2-c1));
        printf("sbrk: %p\n", sbrk(0));
        return 0;
}
