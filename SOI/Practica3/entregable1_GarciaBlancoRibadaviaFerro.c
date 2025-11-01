// Código de Marcos  García Blanco y Sara Ribadavia Ferro
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

// Variables globales
double ultimoValor = 0.0; // Almacena el último valor de la sucesión calculado
int n = 0; // Contador para la sucesión
int running = 1; // Controla la ejecución del programa, cuando es 0 es que se detiene el programa

// Manejador para SIGALRM: Muestra el último valor calculado cuando pasa 1 segundo, la alarma expira
void manejadorAlarma(int sig) {
    printf("Último valor calculado: %d^(1/2) = %.15f\n", n-1, ultimoValor);
    alarm(1);  // Reprogramar la alarma
}

// Manejador para SIGUSR1: Termina el programa cuando el usuario envia SIGUSR1 por la terminal
void manejadorUsr1(int sig) {
    printf("\nSeñal SIGUSR1 recibida. Finalizando programa...\n");
    running = 0; //Detener el bucle principal 
    exit(0);
}

int main() {
    printf("Programa iniciado. PID: %d\n", getpid());  // Mostrar PID del proceso
    printf("Calculando la sucesión n^(1/2) infinitamente...\n");
    printf("Para terminar: kill -SIGUSR1 %d\n\n", getpid());
    
    // Configurar manejadores de señales
    // Asocia la señal SIGALRM con la función manejadorAlarma para que se ejecute cada vez que la alarma expire
    signal(SIGALRM, manejadorAlarma); 
    // Asocia la señal SIGUSR1 con la función manejadorUsr1 para que se ejecute cuando el usuario envíe SIGUSR1 por terminal
    signal(SIGUSR1, manejadorUsr1);
    
    alarm(1);  // Primera alarma
    
    // Bucle principal de cálculo
    while (running) {
        ultimoValor = pow(n, 0.5);  // n^(1/2)
        n++; // Incrementar n para el siguiente cálculo
        pause();  // Esperar por señales
        // Cuando llega SIGALRM: se ejecuta manejadorAlarma, muestra el valor, reprograma alarm(1) y pause() termina
        // De esta forma solo consume CPU cuando llega una señal y no esta calculando constantemente
    }
    
    return 0;
}