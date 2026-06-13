#include <stdio.h>


int get_int(int *a, int *b)
{
	printf("Please enter two number, seprate with space:\n");
	return scanf("%d %d", a, b);
}


