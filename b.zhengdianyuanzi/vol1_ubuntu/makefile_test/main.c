#include <stdio.h>

#include "get_input.h"
#include "calculate.h"


int main(int argc, char *argv[])
{
	int a, b;
	int sum;

	get_int(&a, &b);
	sum = calculate_sum(a, b);
	printf("%d + %d = %d\n\n", a, b, sum);


}
