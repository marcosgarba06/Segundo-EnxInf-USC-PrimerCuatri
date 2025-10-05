#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    double resultado = 0.0;
    int N=1000000, tam = 1000000;
    for (int i =0;i<N;i++)
    {
        for ( int j = 0; j < tam; j++)
        {
            resultado += (double)sqrt(j);           
        }     
    }
    printf("Resultado: %f\n", resultado);
    return 0;
}
