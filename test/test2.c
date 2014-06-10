#include <stdio.h>
#include <psmalloc.h>
#include "core_conf.h"


int main(void)
{
        void *p[10];
        struct chunk_head *ch = NULL;
        
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
}
