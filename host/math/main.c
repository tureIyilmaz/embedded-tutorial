

#include <stdlib.h>
#include <stdio.h>
#include "math.h"


int main()

{
	printf("Product= %I64x\n",math_mul(0X0FFF1111,0X0FFF1111));
	printf("Division= %f\n",math_div(100,8));

	return 0;
}
