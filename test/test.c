#include <stdio.h>
#include <pthread.h>
#include "core_config.h"
void func(void) {
        struct chunk_head *ch, *old_ch;
        int i;
        void *p[10];

        for (i=0; i<10; ++i) {
                p[i] = malloc(i*i + 100);
                //printf("malloc %p\n", p[i]);
        }

        for (i=0; i<10; ++i)
                p[i] = realloc(p[i], i*i + 99);

        for (i=0; i<10; ++i)
                free(p[i]);

        ch = p[0] - chunk_head_size;
        printf("%p, %d, %d, %zu\n", ch, ch->kind, ch->num, ch->seek);

        pthread_exit(0);
}

int main(void)
{
        void *p[10];
        struct chunk_head *ch = NULL;
        printf("sta sbrk %p\n", sbrk(0));
        pthread_t pt;
        /*
        p[0] = ps_malloc(200);
        ch = p[0] - chunk_head_size;
        printf("0 u\n %p\n %d\n %d\n %p\n", ch, ch->kind, ch->num, ch->seek);
        ch = (void*)ch + chunk_size[ch->kind] * ch->num;
        printf("0 f\n %p\n %d\n %d\n %p\n", ch, ch->kind, ch->num, ch->seek);

        p[1] = ps_calloc(20, 40);
        ch = p[1] - chunk_head_size;
        printf("1 u\n %p\n %d\n %d\n %p\n", ch, ch->kind, ch->num, ch->seek);
        ch = (void*)ch + chunk_size[ch->kind] * ch->num;
        printf("1 f\n %p\n %d\n %d\n %p\n", ch, ch->kind, ch->num, ch->seek);

        ps_free(p[0]);
        ch = p[0] - chunk_head_size;
        printf("2 u\n %p\n %d\n %d\n %p\n", ch, ch->kind, ch->num, ch->seek);
        ch = (void*)ch + chunk_size[ch->kind] * ch->num;
        printf("2 f\n %p\n %d\n %d\n %p\n", ch, ch->kind, ch->num, ch->seek);

        ps_free(p[1]);
        ch = p[0] - chunk_head_size;
        printf("3 u\n %p\n %d\n %d\n %p\n", ch, ch->kind, ch->num, ch->seek);
        */
        pthread_create(&pt, NULL, (void*)func, NULL);
        pthread_join(pt, NULL);
        printf("end sbrk %p\n", sbrk(0));

        return 0;
}
