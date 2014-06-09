#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <psmalloc.h>
#include "common.h"

#define test(x) (#x)

int main(void)
{
        struct chunk_head *ch;
        size_t size = sizeof(struct chunk_head);
        void *p = ps_calloc(10, 9);
        

        ch = p - size;
        printf("%zu\nkind = %d\nnum = %d\nseek = %zu\nnext = %x\n", (p-size), ch->kind, ch->num, ch->seek, ch->next);
        ps_free(p);
        printf("%zu\nkind = %d\nnum = %d\nseek = %zu\nnext = %x\n", (p-size), ch->kind, ch->num, ch->seek, ch->next);
        
        return 0;
}
