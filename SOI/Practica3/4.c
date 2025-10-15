#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
int contador = 0;

void manejador (int sig){
    contador++;
    if (contador <= 3){
        printf("Señal SIGINT atrapada y desactivada\n");
        
    }else{
        char respuesta;
        // Después de 3 veces, preguntamos al usuario
        printf("\n¿Restaurar el funcionamiento habitual de Ctrl+C? (s/n): ");
        
        scanf(" %c", &respuesta); // El espacio antes de %c ignora espacios en blanco
        
        if (respuesta == 's' || respuesta == 'S') {
            printf("Restaurando comportamiento normal de Ctrl+C...\n");
            // Restauramos el manejo por defecto de SIGINT
            signal(SIGINT, SIG_DFL);
        } else {
            printf("Continuando con Ctrl+C desactivado...\n");
        }
    }
    
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, manejador); //SIGINT es la señal que se envía al presionar Ctrl+C
    
    while(1){
        printf("Programa ejecutándose (PID: %d)...\n", getpid());
        sleep(5);
    }
    return 0;
}
