#include "common.h"
#include <stdio.h>

void foo(enum chunk_kind kind)
{
        if (kind > small)
                printf("%d\n", kind);
}

int main(void)
{
        foo(big);
        return 0;
}
