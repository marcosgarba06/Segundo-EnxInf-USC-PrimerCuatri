#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

// Variables globales
double ultimoValor = 0.0;
int n = 0;
int running = 1;

// Manejador para SIGALRM - muestra el último valor calculado
void manejadorAlarma(int sig) {
    printf("Último valor calculado: %d^(1/2) = %.15f\n", n-1, ultimoValor);
    alarm(1);  // Reprogramar la alarma
}

// Manejador para SIGUSR1 - termina el programa
void manejadorUsr1(int sig) {
    printf("\nSeñal SIGUSR1 recibida. Finalizando programa...\n");
    running = 0;
    exit(0);
}

int main() {
    printf("Programa iniciado. PID: %d\n", getpid());
    printf("Calculando la sucesión n^(1/2) infinitamente...\n");
    printf("Para terminar: kill -SIGUSR1 %d\n\n", getpid());
    
    // Configurar manejadores de señales
    signal(SIGALRM, manejadorAlarma);
    signal(SIGUSR1, manejadorUsr1);
    
    alarm(1);  // Primera alarma
    
    // Bucle principal de cálculo
    while (running) {
        ultimoValor = pow(n, 0.5);  // n^(1/2) = raíz cuadrada de n
        n++;
        pause();  // Esperar por señales
    }
    
    return 0;
}