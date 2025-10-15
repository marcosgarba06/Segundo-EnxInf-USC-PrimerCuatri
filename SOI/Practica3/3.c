#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/* Incluye las librerias necesarias*/
static void gestion(int); /*Declaracion de la funcion de gestion de señales recibidas*/

int main() {
    /* Declara las variables necesarias*/
    pid_t padre,hijo;

    if (signal(SIGUSR1, gestion) == SIG_ERR) printf("Error al crear gestor 1\n");
    if (signal(SIGUSR2, gestion) == SIG_ERR) printf("Error al crear gestor 2\n");

    padre=getpid();


    if ((hijo = fork()) == 0) { /*Trabajo del hijo*/
        printf("Soy el proceso hijo(PID %d) y mi padre es %d\n", getpid(), padre);

        kill(padre,SIGUSR1);
        for (;;); /*Espera señal del padre indefinidamente*/
    }
    else { /*Trabajo del padre*/
    /* Completa el código del padre para enviar las señales SIGUSR2 y SIGTERM al hijo,
    y espera a que acabe con wait */
        printf("Soy el padre mi PID es %d, el PID del hijo es %d\n", padre, hijo);

        sleep(3); //espera 3 segundos para que el hijo pueda ejecutar su código
        kill(hijo,SIGUSR2); //SIGNAL USER 2, definida por el usuario

        sleep(3); //espera 3 segundos para ver por pantalla la señal del SIGUSR2 antes de matar al hijo
        kill(hijo,SIGTERM); 

        wait(NULL);

        printf("El hijo ha terminado, el padre %d termina también\n",padre);
        exit(0); /*Termina el padre*/
    }
}

    static void gestion(int numero_de_senhal) { /* Funcion de gestion de señales*/  
        switch (numero_de_senhal) {
            case SIGUSR1: /*Entra señal SIGUSR1*/
            printf("Señal tipo 1 recibida. Soy %d\n",getpid()); break;
                /*Completa para el resto de las señales usadas*/
            case SIGUSR2: /*Entra señal SIGUSR2*/
            printf("Señal tipo 2 recibida. Soy %d\n",getpid()); break;

            default: printf("Señal no manejada %d\n",numero_de_senhal); break;
        }
    return; 
}