#include <stdio.h>
#include <malloc.h>
#include <pthread.h>

void func(void)
{
        printf("in thread\n");
}

int main(void)
{
        pthread_t tid;
        pthread_create(&tid, NULL, (void*)func, NULL);
        pthread_join(tid, NULL);
        printf("success\n");

        return 0;
}
