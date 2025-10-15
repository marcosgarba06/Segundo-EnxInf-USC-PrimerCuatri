#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int num;
    printf("PID del proceso: %d\n", getpid());
    printf("Ingrese un numero (el programa esperará indefinidamente): ");
    scanf(" %d", &num);

    printf("El numero ingresado es: %d\n", num);
    printf("El programa ha terminado.\n");
    return 0;
}
