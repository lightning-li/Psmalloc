#include <malloc.h>
#include <stdio.h>
#include <pthread.h>

int main(void)
{
        void *p = malloc(100);
        free(p);

        return 0;
}
