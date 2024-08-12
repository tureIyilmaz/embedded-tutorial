/*
 * main.c
 *
 *  Created on: 11 Eki 2022
 *      Author: turel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
char a[] = "Turel";
int size = strlen(a);


for(int i=0;i<size;i++)
{
	printf("%p\n",&a[i]);
}

return 0;
}
