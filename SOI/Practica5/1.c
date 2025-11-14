// Ejercicio 1
// Código de Marcos García Blanco e Inés Landeira Rodríguez
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Variables globales
int global_int = 10;
int global_array [5] = {1, 2, 3, 4, 5};
char global_char = 'G';
int global_uninit; // Variable global no inicializada

int main(int argc, char const *argv[])
{
    // Variables locales
    int local_var = 200;
    char local_char = 'L';
    int local_uninit; // Variable local no inicializada
    
    // Array 3D local
    int local_array_3d[10][10][10];

    int i, j, k;

    // Inicializar valores del array 3D local
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            for (k = 0; k < 10; k++) {
                local_array_3d[i][j][k] = i * 100 + j * 10 + k;
            }
        }
    }

    int pid = getpid();
    printf("PID: %d\n", pid);
    
    printf("Para ver el mapa de memoria, ejecuta:cat /proc/%d/maps\n", pid);
    printf("O usa 'watch' para verlo en tiempo real: watch -n 1 cat /proc/%d/maps\n", pid);

    // Imprimir en hexadecimal las direcciones de las variables 
    // Se imprime con %p que es el especificador para punteros, es decir imprime en hexadecimal el valor de las direcciones de memoria
    printf("Función main:              %p\n", &main); // con main obtenemos la dirección de la función main
    printf("\n");
    
    // Direcciones de variables globales
    printf("Variables globales:\n");
    printf("  global_int:              %p\n", &global_int);
    printf("  global_array:            %p\n", &global_array);
    printf("  global_char:             %p\n", &global_char);
    printf("  global_uninit:           %p\n", &global_uninit);
    printf("\n");
    
    // Direcciones de variables locales
    printf("Variables locales:\n");
    printf("  local_var:               %p\n", &local_var);
    printf("  local_char:              %p\n", &local_char);
    printf("  local_uninit:            %p\n", &local_uninit);
    printf("  local_array_3d:          %p\n", &local_array_3d);
    printf("\n");

    // Comprobar el orden de almacenamiento del array 3D en memoria 
    // Aquí imprimimos direcciones de elementos específicos para demostrar el orden.
    // La distancia entre [0][0][0] y [0][1][0] es 10*4 = 40 bytes (10 elementos en k).
    // La distancia entre [0][0][0] y [1][0][0] es 100*4 = 400 bytes (10*10 elementos en j y k).
    // Esto confirma que k varía más rápido, luego j, luego i.
    /*Esto significa que en memoria, los elementos se almacenan en este orden secuencial:

        [0][0][0] (primero).
        [0][0][1] (cambia k).
        ...
        [0][0][9] (completa el grupo de k para i=0, j=0).
        
        Luego salta a [0][1][0] (cambia j).
        [0][1][1], ..., [0][1][9] (completa el grupo de k para i=0, j=1).
        Y así hasta [0][9][9] (completa todos los j para i=0).
        Luego salta a [1][0][0] (cambia i).
        Y continúa hasta [9][9][9].*/
        
    printf("Orden de almacenamiento del array 3D (elementos consecutivos en memoria):\n");
    printf("  local_array_3d[0][0][0]: %p\n", &local_array_3d[0][0][0]);
    printf("  local_array_3d[0][0][1]: %p\n", &local_array_3d[0][0][1]);
    printf("  local_array_3d[0][1][0]: %p\n", &local_array_3d[0][1][0]);
    printf("  local_array_3d[1][0][0]: %p\n", &local_array_3d[1][0][0]);
    printf("\n");
   
    char wait;
    scanf("%c", &wait); // Espera una entrada para pausar la ejecución y permitir al usuario ver el mapa de memoria
    return 0;
}