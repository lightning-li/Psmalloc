#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>

void func(void)
{
        const int num = 15;
        int i = 0;
        void *p1[num];
        void *p2[num];

        for (i=0; i<num; ++i)
                p1[i] = malloc(i*i*100 + 100);
        
        for (i=0; i<num; ++i)
                p1[i] = realloc(p1[i], i*i*100 + 200);

        for (i=0; i<num; ++i)
                p2[i] = malloc(i*i*100 + 100);

        for (i=0; i<num; ++i)
                free(p1[i]);

        for (i=0; i<num; ++i)
                p2[i] = realloc(p2[i], i*i*100 + 60);
        
        for (i=0; i<num; ++i)
                free(p2[i]);

        pthread_exit(0);
}

int main(void)
{
        const int num = 100;
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
        printf("time: %.2lf ms\n", (double)(c2-c1)/1000);
        
        return 0;
}
