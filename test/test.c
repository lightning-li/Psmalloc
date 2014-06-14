#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include "core_config.h"
void func(void)
{
        const int num = 1;
        int i = 0;
        void *p1[num];
        void *p2[num];

        for (i=0; i<num; ++i){
                p1[i] = malloc(i*i*100 + 100);
                printf("malloc  %p\n", p1[i] - chunk_head_size);
        }
        //for (i=0; i<num; ++i)
        //p1[i] = realloc(p1[i], i*i*100 + 200);

        //for (i=0; i<num; ++i)
        //p2[i] = malloc(i*i*100 + 100);

        //for (i=0; i<num; ++i)
        //free(p1[i]);

        //for (i=0; i<num; ++i)
        //p2[i] = realloc(p2[i], i*i*100 + 60);

        //for (i=0; i<num; ++i)
        //free(p2[i]);

        pthread_exit(0);
}

void func1(void) {
        const int num = 3;
        int i = 0;
        void *p1[num];
        for (i=0; i<num; ++i){
                p1[i] = malloc(i*i*100 + 100);
                printf("malloc  %p\n", p1[i] - chunk_head_size);
        }

        pthread_exit(0);
}

int main(int argc, char *argv[])
{
        int i, j;
        clock_t cl;
        double call_time = 0;
        int num = atoi(argv[1]);
        pthread_t tid[num];

        for (j=0; j<1; ++j) {
                cl = clock();
                for (i=0; i<num; ++i)
                        pthread_create(&tid[i], NULL, (void*)func, NULL);
                
        
                for (i=0; i<num; ++i)
                        pthread_join(tid[i], NULL);

                pthread_create(&tid[1], NULL, (void*)func1, NULL);
                pthread_join(tid[1], NULL);
                call_time += (clock() - cl)/1000;
        }
        printf("time: %.2lf ms\n", call_time/10);
        
        return 0;
}
