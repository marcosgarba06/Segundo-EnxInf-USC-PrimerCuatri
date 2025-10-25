#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void manejador(int sig){
    printf("Señal %d recibida por el proceso con PID: %d\n", sig, getpid());
}

int main(int argc, char const *argv[])
{
    pid_t pid;

    //Crear  hijo
    pid = fork();
 
    if(pid < 0){
        printf("Error al crear el proceso hijo\n");
        exit(1);
    }else if(pid == 0){
        //Codigo del hijo
        printf("Hola, soy el proceso hijo con PID: %d\n", getpid());

     // pause() bloquea el proceso hasta que reciba una señal
        pause();
        
        printf("Hijo (PID: %d): Saliendo del pause()\n", getpid());
        printf("Hijo (PID: %d): Finalizando ejecución\n", getpid());
        
        exit(EXIT_SUCCESS);
    } else {
        // Código del proceso padre
        printf("Padre: Proceso hijo creado con PID: %d\n", pid);
        
        // Esperar 5 segundos
        printf("Padre (PID: %d): Esperando 5 segundos...\n", pid);
        sleep(5);
        
        // Enviar señal SIGUSR1 al hijo
        printf("Padre: Enviando señal SIGUSR1 al hijo (PID: %d)\n", pid);
        if (kill(pid, SIGUSR1) == -1) {
            perror("Error al enviar señal");
        }
        
        // Esperar a que el hijo termine
        wait(NULL);
        printf("Padre: Proceso hijo ha terminado. Finalizando.\n");
    }
    
    return 0;
}