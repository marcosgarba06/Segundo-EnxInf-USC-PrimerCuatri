#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int *p;
	int i,j,k=0,tam=2000000,N=400;

	for(i=0; i<N; i++)
	{
		p=(int*)malloc(tam * sizeof(int));
		for(j=0;j<10000;j+=322)
		{
			p[j]=i%39;
			k++;
		}
	
		sleep(1);
	}
	scanf("%d", &i);
	printf("%d\n",i);
}

