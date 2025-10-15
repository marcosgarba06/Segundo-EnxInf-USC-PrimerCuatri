#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(void){
    printf("[%d] Soy el padre\n", getpid());
    int salida;

    pid_t H1 = fork();
    if(H1 < 0){
        printf("Error creando el primer proceso hijo (H1)\n");
        exit(1);
    }else if(H1 == 0){
        printf("[%d] Hola soy H1\n", getpid());
        sleep(5);

        printf("[%d] H1 finaliza con código de salida 25\n", getpid());
        exit(25);
    }else{
        pid_t H2 = fork();
        if(H2 < 0){
            printf("Error creando el segundo proceso hijo (H2)\n");
            exit(1);
        }else if(H2 == 0){
            char respuesta;
            printf("[%d] Hola soy H2\n", getpid());
            printf("¿Desea finalizar el proceso H1? (s/n)");
            scanf(" %c", &respuesta);
            if (respuesta == 's'|| respuesta == 'S'){
                kill(H1, SIGTERM);
                printf("Proceso H1 finalizado\n");
                printf("H2 finaliza tambien\n");
                exit(EXIT_SUCCESS);
            }else{
                printf("Proceso H1 no finalizado\n");
            }
         
        }else{
            //Codigo del padre
            waitpid(H1, &salida, 0);
            waitpid(H2, NULL, 0);
            printf("El padre %d finaliza\n", getpid());
            exit(EXIT_SUCCESS);
        }
    }
}