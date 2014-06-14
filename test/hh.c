#include <malloc.h>

int main(void)
{
        void *p = malloc(1024*512);
        free(p);
        return 0;
}
