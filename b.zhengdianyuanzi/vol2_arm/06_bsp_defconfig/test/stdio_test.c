#include "stdio_test.h"

#include "stdio.h"

void stdio_io_format_test(void)
{
    int a, b;
    printf("Please enter 2 num, split with space:");
    scanf("%d %d", &a, &b);
    printf("\n%d+%d=%d\n\n\n\n", a, b, a + b);

    return;
}