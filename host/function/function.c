/*
 * function.c
 *
 *  Created on: 12 Eki 2022
 *      Author: turel
*/


#include <stdlib.h>
#include <stdio.h>


int func_add(int *, int *, int *);

int main()

{
	int n1,n2,n3;

	scanf("%d %d %d",&n1,&n2,&n3);

	printf("sum = %d",func_add(&n1,&n2,&n3));

	return 0;
}


int func_add(int *a , int *b , int *c)

{
	int sum;
	sum= *a+*b+*c;
return sum;
}
