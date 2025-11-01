#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t hilo1; // Variable global para que el hilo 2 pueda esperar al hilo 1

// Función del primer hilo (duerme más tiempo)
void* funcHilo1(void* arg) {
    int segundos = 5;
    printf("Hilo 1: Voy a dormir %d segundos\n", segundos);
    sleep(segundos);
    printf("Hilo 1: He dormido %d segundos. Terminando.\n", segundos);
    pthread_exit(NULL);
}

// Función del segundo hilo (duerme menos tiempo)
void* funcHilo2(void* arg) {
    int segundos = 2;
    printf("Hilo 2: Voy a dormir %d segundos\n", segundos);
    sleep(segundos);
    
    // CLAVE: El hilo 2 espera a que el hilo 1 termine con pthread_join
    // pthread_join bloquea es una funcion que espera a que un hilo termine su trabajo para continuar
    //con la ejecucion del hilo que la llama
    pthread_join(hilo1, NULL);
    
    printf("Hilo 2: He dormido %d segundos. Terminando.\n", segundos);
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    pthread_t hilo2;
    int res;

    // Crear el primer hilo con pthread_create
    res = pthread_create(&hilo1, NULL, funcHilo1, NULL);
    if (res != 0) {
        perror("Error al crear el hilo 1");
        exit(EXIT_FAILURE);
    }

    // Crear el segundo hilo con pthread_create
    res = pthread_create(&hilo2, NULL, funcHilo2, NULL);
    if (res != 0) {
        perror("Error al crear el hilo 2");
        exit(EXIT_FAILURE);
    }

    // El main espera al hilo 2, que internamente espera al hilo 1, por lo que
    // el main esperará a que ambos hilos terminen antes de continuar. y terminar
    // con la ehecucion del programa
    printf("MAIN: Esperando a que los hilos terminen...\n");
    res = pthread_join(hilo2, NULL);
    if (res != 0) {
        perror("Error al hacer join del hilo 2");
        exit(EXIT_FAILURE);
    }

    // Nota: pthread_join dentro del hilo 2 garantiza que su mensaje
    // se imprime SIEMPRE después del mensaje del hilo 1.
    
    exit(EXIT_SUCCESS);
}