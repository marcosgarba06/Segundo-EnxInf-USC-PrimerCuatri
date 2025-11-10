#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
 * Función ejecutada por los hilos 1 y 2
 * Estos hilos usan pthread_exit() para terminar solo su ejecución
 */
void *funcionPthread_Exit(void *arg) {
    // Convertir el argumento recibido a entero
    int n = *(int *)arg;
    printf("Hilo %d creado\n", n);

    // Dormir 20 segundos (trabajo largo)
    // Esto garantiza que el hilo 3 ejecute exit() antes
    sleep(20);

    //Como exit termina el programa este mensaje no se deberia de ejecutar
    printf("Mensaje que no debería aparecer\n");


    // pthread_exit() solo terminaría ESTE hilo (no todo el proceso)
    // Pero nunca se alcanza porque exit() mata el proceso antes
    pthread_exit(NULL);
}

/**
 * Función ejecutada por el hilo 3
 * Este hilo usa exit() para terminar TODO EL PROCESO
 */
void *funcionExit(void *arg) {
    // Convertir el argumento recibido a entero
    int n = *(int *)arg;

    printf("Hilo %d creado\n", n);

    // Dormir solo 1 segundo
    // Esto garantiza que exit() se ejecute antes de que
    // los hilos 1 y 2 completen sus 20 segundos
    sleep(1);

    printf("El hilo %d va a terminar el proceso\n", n);
    
    // exit() termina TODO EL PROCESO, incluidos todos los hilos
    // NO se ejecuta ningún código posterior a esta llamada
    // Los hilos 1 y 2 serán terminados forzosamente
    exit(1);
}

int main() {
    pthread_t hilos[3];
    int i = 1, j = 2, z = 3;
    int res;

    res = pthread_create(&hilos[0], NULL, funcionPthread_Exit, &i);
    if (res != 0) {
        fprintf(stderr, "Error al crear el hilo 1\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&hilos[1], NULL, funcionPthread_Exit, &j);
    if (res != 0) {
        fprintf(stderr, "Error al crear el hilo 2\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&hilos[2], NULL, funcionExit, &z);
    if (res != 0)
    {
        fprintf(stderr, "Error al crear el hilo 3\n");
        exit(EXIT_FAILURE);
    }

    // Esperar a que los hilos terminen para evitar que el proceso termine antes
    for (int k = 0; k < 3; k++) {
        pthread_join(hilos[k], NULL);
    }
    
    exit(EXIT_SUCCESS);
}