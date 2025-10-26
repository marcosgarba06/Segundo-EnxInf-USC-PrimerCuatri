#define _POSIX_C_SOURCE 200809L  //Habilita las funciones POSIX estándar (como sigaction)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>

/* Declaración de la función de gestión de señales recibidas */
static void gestion(int);


void horaActual(char *buffer, size_t tam) {
    struct timeval tv; // Estructura para tiempo con microsegundos
    struct tm *tm_info; // Estructura para tiempo desglosado
    
    gettimeofday(&tv, NULL); // Obtener tiempo actual con microsegundos
    
    tm_info = localtime(&tv.tv_sec); // Convertir de Epoch (segundos desde 1-1-1970) a tiempo local desglosado
    
    snprintf(buffer, tam, "%02d:%02d:%02d.%06ld", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec, tv.tv_usec);  //Formato HH:MM:SS.mmmmmm
}


int main(){
    char hora[20]; // Cadena para almacenar la hora con milisegundos
    pid_t padre, H1, N1; 
    struct sigaction sa; //Estructura de sigaction 
    /* Estructura de sigaction 
    campos: 
        sa_handler: puntero a la funcion manejador
        sa_mask: conhunto de señales a bloquear durante la ejecución del manejador
        sa_flags: int para las flags de comportamiento
    */    
    sigset_t mask, pending; // Conjuntos de señales para bloqueo y consulta de pendientes
    
    padre = getpid(); 

    horaActual(hora, sizeof(hora));
    printf("[%d] [%s] Hola, soy el proceso padre\n", padre, hora);
    
    /* CONFIGURACIÓN DEL MANEJADOR DE SEÑALES */
    sa.sa_handler = gestion;  // Función manejadora
    sigemptyset(&sa.sa_mask); // Máscara vacía
    sa.sa_flags = SA_RESTART; // Flag para reiniciar llamadas al sistema
    
    // Manejador para SIGUSR1 y comprobación de errores
    if (sigaction(SIGUSR1, &sa, 0) == -1) { 
        printf("Error al crear gestor para SIGUSR1\n");
        exit(1);
    }
    
    // Manejador para SIGUSR2 y comprobación de errores
    if (sigaction(SIGUSR2, &sa, 0) == -1) { 
        printf("Error al crear gestor para SIGUSR2\n");
        exit(1);
    }
    
    horaActual(hora, sizeof(hora));
    printf("[%d] [%s] Gestores configurados\n", padre, hora);
    
    /* BLOQUEO DE SIGUSR1 USANDO sigprocmask */
    // Se bloquea antes del fork para que no haya pelea entre el hijo y el bloqueo de SIGUSR1.
    // De esta forma se asegura el bloqueo de SIGUSR1
    if (sigprocmask(SIG_BLOCK, &mask, 0) == -1) {
        printf("Error al bloquear SIGUSR1\n");
        exit(1);
    }

    horaActual(hora, sizeof(hora));
    printf("[%d] [%s] SIGUSR1 bloqueada\n", padre, hora);
    
    /* CREACIÓN DEL PROCESO HIJO H1 */
    H1 = fork();
    
    if(H1 < 0){
        printf("Error creando hijo\n");
        exit(1);
        
    } else if(H1 == 0){
        /* CÓDIGO DEL HIJO H1 */
        pid_t hijo = getpid();

        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] Soy H1\n", hijo, hora); //Identificación del proceso hijo
        
        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (H1) Enviando SIGUSR1 al padre %d\n", hijo, hora, padre); //H1 envía SIGUSR1 al padre
        kill(padre, SIGUSR1); //H1 envía SIGUSR1 a P que tiene que quedar pendiente por estar bloqueada
        
        /* H1 CREA NIETO N1 */
        N1 = fork();
        
        if(N1 < 0){
            printf("Error creando nieto\n");
            exit(1);
            
        } else if(N1 == 0){
            /* CÓDIGO DEL NIETO N1 */
            pid_t nieto = getpid();

            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] Soy N1\n", nieto, hora); //Identificación del proceso nieto
            
            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (N1) Enviando SIGUSR2 al abuelo %d\n", nieto, hora, padre); //N1 envía SIGUSR2 al abuelo
            kill(padre, SIGUSR2); // N1 envía SIGUSR2 al abuelo (P) que no está bloqueada, se deberia recibir inmediatamente

            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (N1) Durmiendo 5s\n", nieto, hora);
            sleep(5); //Espera de 5 segundos
            
            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (N1) exit(38)\n", nieto, hora);
            exit(38); //Termina
            
        } else {
            /* H1 ESPERA A N1 */
            int statusN;
            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (H1) Esperando a N1 (PID:%d)\n", hijo, hora, N1);
            
            wait(&statusN);
            
            int codigoN = WEXITSTATUS(statusN);

            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (H1) N1 devolvio: %d\n", hijo, hora, codigoN); //Identificación del código de salida de N1
        }
        
        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (H1) exit(25)\n", hijo, hora);
        exit(25);
        
    } else {
        /* CÓDIGO DEL PADRE P */

        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (Padre) H1 creado (PID:%d)\n", padre, hora, H1);

        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (Padre) Durmiendo 10s\n", padre, hora);
        sleep(10); //El padre espera 10 segundos a que el nieto envie sigusr2
        
        /* CONSULTA DE SEÑALES PENDIENTES */
        //Se hacen despues del sleep porque hay que dar tiempo a que H1 y N1 envien las señales
        if (sigpending(&pending) == -1) {// Consulta el conjunto de señales pendientes (bloqueadas y recibidas)
            printf("Error en sigpending\n");
            exit(1);
        }
        
        if (sigismember(&pending, SIGUSR1)) { // Comprueba si SIGUSR1 está pendiente (debería estarlo al estar bloqueada)
            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (Padre) SIGUSR1 esta pendiente\n", padre, hora);
        } else {
            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (Padre) SIGUSR1 NO pendiente\n", padre, hora);
        }

        if (sigismember(&pending, SIGUSR2)) { // Comprueba si SIGUSR2 está pendiente (debería NO estarlo al no estar bloqueada)
            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (Padre) SIGUSR2 esta pendiente\n", padre, hora);
        } else {
            horaActual(hora, sizeof(hora));
            printf("[%d] [%s] (Padre) SIGUSR2 NO pendiente\n", padre, hora);
        }

        /* DESBLOQUEO DE SIGUSR1 */
        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (Padre) Desbloqueando SIGUSR1\n", padre, hora); 
        if (sigprocmask(SIG_UNBLOCK, &mask, 0) == -1) {  //Desbloquea SIGUSR1, la señal pendiente se entrega y se ejecuta el manejador
            printf("Error al desbloquear\n");
            exit(1);
        }

        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (Padre) SIGUSR1 desbloqueada\n", padre, hora);
        
        sleep(1); //Espera para que el mensaje del mamnejador aparezca antes de seguir el programa
        
        /* ESPERA A H1 E IMPRIME SU CODIGO DE SALIDA */
        int status;

        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (Padre) Esperando a H1 (PID:%d)\n", padre, hora, H1);
        wait(&status);
        
        int codigo = WEXITSTATUS(status);
        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (Padre) H1 devolvio: %d\n", padre, hora, codigo);
        
        /* FINALIZACIÓN DEL PADRE */
        horaActual(hora, sizeof(hora));
        printf("[%d] [%s] (Padre) exit(0)\n", padre, hora);
        exit(0);
    }
    
    return 0;
}

/* FUNCIÓN MANEJADORA */
static void gestion(int numero_de_senhal) {
    char hour[20];
    switch (numero_de_senhal) {
        case SIGUSR1:
            horaActual(hour, sizeof(hour));
            printf("[%d] [%s]: SIGUSR1 recibida\n", getpid(), hour);
            break;
            
        case SIGUSR2:
            horaActual(hour, sizeof(hour));
            printf("[%d] [%s]: SIGUSR2 recibida\n", getpid(), hour);
            break;
    }
}