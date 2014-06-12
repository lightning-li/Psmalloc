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
                p1[i] = ps_malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                p1[i] = ps_realloc(p1[i], i*i*100 + 101);

        //printf("\n2malloc\n");
        for (i=0; i<num; ++i) {
                p2[i] = ps_malloc(i*i*100 + 100);
                //printf("ps_malloc ptr %p\n", p2[i]);
        }

        //printf("\n1free\n");
        for (i=0; i<num; ++i)
                ps_free(p1[i]);

        //printf("\n2realloc\n");
        for (i=0; i<num; ++i)
                p2[i] = ps_realloc(p2[i], i*i*100 + 101);

        //printf("\n2free\n");
        for (i=0; i<num; ++i)
                ps_free(p2[i]);

        //ch = p1[0] - chunk_head_size;
        //printf("%p, %d, %d, %zu\n", ch, ch->kind, ch->num, ch->seek);

        pthread_exit(0);
}

int main(void)
{
        const int num = 1500;
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
        printf("ps:\n");
        printf("time: %lf\n", (double)(c2-c1));
        printf("sbrk: %p\n", sbrk(0));

        
        return 0;
}
