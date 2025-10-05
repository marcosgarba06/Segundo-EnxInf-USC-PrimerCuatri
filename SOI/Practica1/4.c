#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    clock_t startTimer, endTimer;
    startTimer = clock();
     
    FILE *f = fopen("archivo4.txt", "w");
    
    if (f == NULL) { //Comprobar que el archivoe se ha abierto
        perror("No se pudo abrir el archivo");
        return 1;
    }

    long N = 100000000; // número de escrituras

    startTimer = clock(); //tiempo al inicio

    for (long i = 0; i < N; i++) {
        fprintf(f, "Linea numero %ld\n", i);
    }

    endTimer = clock(); //tiempo al final
    double tiempo = ((double)(endTimer - startTimer)) / CLOCKS_PER_SEC;
    double operacionesPorSeg = N / tiempo;
    printf("Tiempo total: %f segundos\n", tiempo);
    printf("Operaciones por segundo: %f\n", operacionesPorSeg);

    return 0;
}
