#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int* a[1000];
	for(int i=0; i<1000; i++)
	{
		a[i] = (int*)malloc(sizeof(int)*200000000);
		for(int j=0; j<200000; j+=344)
			a[i][j]=j-i;
		sleep(1);
	}
	scanf("%c");
}

//Para sacar el archivo en ensamblador se utuliza: gcc -S 1_1.c, es decir se utuliza la opcion -S del compilador gcc