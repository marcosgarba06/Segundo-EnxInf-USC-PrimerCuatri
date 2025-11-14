// Ejercicio 3
// Código de Marcos García Blanco e Inés Landeira Rodríguez
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h> // Para alloca
#include <unistd.h> // Para getpid

int main(int argc, char const *argv[])
{
    printf("Puedes comprobar el mapa de memoria usando cat /proc/%d/maps\n", getpid());
    //Esperar a que el usuario pulse una tecla para que 
    printf("Pulse enter para ejecutar un malloc de 100 bytes:");
    scanf("%*c"); // Esperar a que el usuario pulse enter
    
    // Realizar el malloc de 100 bytes
    void *malloc1;
    malloc1 = malloc(100);

    // Comprobar si el malloc ha sido exitoso
    if (malloc1 == NULL) {
        printf("Error en malloc\n");
        return 1;
    }

    // Imprimir la dirección de memoria reservada
    printf("Memoria reservada en %p\n", malloc1);

    printf("Pulsa enter para hacer un segundo malloc:");
    scanf("%*c");
   
    // Realizar el segundo malloc
    void *malloc2;
    malloc2 = malloc(50);
    
    if (malloc2 == NULL) {
        printf("Error en malloc\n");
        free(malloc1);
        return 1;
    }

    // Imprimir la dirección de memoria reservada
    printf("Memoria reservada en %p\n", malloc2);
    

    // Reservar memoria con alloca para ver las diferencias con malloc
    printf("Pulsa enter para reservar memoria con alloca:");
    scanf("%*c");
    
    void *alloca1;
    alloca1 = alloca(75); // No se puede comprobar si alloca ha sido exitoso ya que no devuelve NULL en caso de error

    printf("Memoria reservada con alloca en %p\n", alloca1);

    // Redimensionar la primera reserva de memoria con realloc a un valor mayor
    printf("Pulsa enter para modificar la primera reserva de memoria con realloc:");
    scanf("%*c");
    
    void *realloc1;
    realloc1 = realloc(malloc1, 1024*1024); // Redimensionar la primera reserva
    if (realloc1 == NULL) {
        printf("Error en realloc\n");
        free(malloc1);
        free(malloc2);
        return 1;
    }
    printf("Memoria redimensionada en %p\n", realloc1);

    // Malloc para reservar espacio para 1000 doubles 
    printf("Pulsa enter para reservar memoria para 1000 doubles con malloc:");
    scanf("%*c");

    double *mallocDouble1;
    mallocDouble1 = malloc(1000*sizeof(double));

    if (mallocDouble1 == NULL) {
        printf("Error en malloc\n");
        free(realloc1);
        free(malloc2);
        return 1;
    }
    printf("Memoria reservada para 1000 doubles en %p\n", mallocDouble1);
    printf("sizeof del puntero devuelto por malloc: %zu bytes\n", sizeof(mallocDouble1));
    
    // Cambiar el puntero antes de llamar a free para ver qué pasa
    printf("Pulsa enter para cambiar el puntero devuelto por malloc:");
    scanf("%*c");    
    
    // Cambiar el puntero, guardar una copia original para poder liberarlo despuésç
    double *originalMallocDouble1 = mallocDouble1;
    mallocDouble1 = mallocDouble1 + 1; // Mover el puntero
    
    // Imprimir ambos punteros para ver las diferencias
    printf("Puntero original: %p\n", originalMallocDouble1);
    printf("Puntero modificado: %p\n", mallocDouble1);

    // Liberar la memoria (se libera el puntero original)
    printf("Pulsa enter para liberar la memoriac de los 1000 doubles:");
    scanf("%*c");
    free(originalMallocDouble1);

    // Liberar las otras reservas de memoria de malloc y realloc
    printf("Pulse enter para liberar la memoria del segundo malloc:");
    scanf("%*c");
    free(malloc2);

    printf("Pulse enter para liberar la memoria del realloc:");
    scanf("%*c");
    free(realloc1);

    printf("Liberación de memoria completada.\n");
    return 0;   
}