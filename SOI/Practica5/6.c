// Marcos García Blanco e Inés Landeira Rodríguez

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/syscall.h>  

// Global -> segmento de datos 
int G = 7;

typedef struct {
    int *pvalor_main;  // puntero a int en heap que contiene el VALOR de la variable local de main
} hilo_args_t;

static void* hilo_func(void *arg) {
    long tid = syscall(SYS_gettid);
    printf("[HILO] PID=%d  TID=%ld  pthread_self=%lu\n", getpid(), tid, (unsigned long)pthread_self());
    fflush(stdout);
    hilo_args_t *in = (hilo_args_t*)arg;

    
    // El parámetro real del hilo (arg) es un parámetro por valor de la función del hilo.
    // Su propia dirección (&arg) está en la PILA de ESTE hilo.
    int param_val = *(in->pvalor_main);  // copiamos el valor 
    int local_hilo = G * param_val;      // local -> pila del hilo

    // Reserva dinámica en el hilo -> [heap] (común al proceso)
    int *buf = (int*)malloc(1024);
    if (buf) *buf = local_hilo;

    // Identificador “humano” del hilo 
    pthread_t self = pthread_self();

    printf("\n[HILO %lu] &G               = %p   (global, datos)\n",
           self, (void*)&G);
    printf("[HILO %lu] &arg (parametro real) = %p   (pila del hilo)\n",
           self, (void*)&arg);
    printf("[HILO %lu] in->pvalor_main  = %p   (puntero al int en heap)\n",
           self, (void*)in->pvalor_main);
    printf("[HILO %lu] &param_val       = %p   (local del hilo, pila)  valor=%d\n",
           self, (void*)&param_val, param_val);
    printf("[HILO %lu] &local_hilo      = %p   (local del hilo, pila)  valor=%d\n",
           self, (void*)&local_hilo, local_hilo);
    printf("[HILO %lu] buf (malloc)     = %p   ([heap])\n",
           self, (void*)buf);

    free(buf);
    // liberamos el contenedor de argumentos y el int del valor
    free(in->pvalor_main);
    free(in);
    fflush(stdout);
    return NULL;
}

int main(void) {
    // Local del hilo principal -> pila principal [stack]
    int main_local = 5;

    printf("PID = %d\n", getpid());
    printf("[MAIN] &G          = %p   (global, datos)\n", (void*)&G);
    printf("[MAIN] &main_local = %p   (pila principal [stack])  valor=%d\n",
           (void*)&main_local, main_local);

    // Preparamos argumentos de cada hilo en HEAP para que la dirección sea estable
    hilo_args_t *a1 = (hilo_args_t*)malloc(sizeof(hilo_args_t));
    hilo_args_t *a2 = (hilo_args_t*)malloc(sizeof(hilo_args_t));
    a1->pvalor_main = (int*)malloc(sizeof(int));
    a2->pvalor_main = (int*)malloc(sizeof(int));
    *(a1->pvalor_main) = main_local;   // reciben como parámetro la variable local del main 
    *(a2->pvalor_main) = main_local;

    // malloc en el main para mostrar también [heap]
    int *p_main = (int*)malloc(2048);
    printf("[MAIN] p_main (malloc) = %p   ([heap])\n", (void*)p_main);

    pthread_t t1, t2;
    int rc1 = pthread_create(&t1, NULL, hilo_func, a1);
    if (rc1 != 0) { fprintf(stderr, "pthread_create t1 fallo: %s\n", strerror(rc1)); exit(1); }
    int rc2 = pthread_create(&t2, NULL, hilo_func, a2);
    if (rc2 != 0) { fprintf(stderr, "pthread_create t2 fallo: %s\n", strerror(rc2)); exit(1); }

    // Insprcción del mapa
    printf("\n--> Abre otra terminal y mira el mapa:\n");
    printf("    cat /proc/%d/maps | less\n", getpid());
    printf("    # Para ver pilas por hilo (cada TID):\n");
    printf("    ls /proc/%d/task\n", getpid());
    printf("    # Y luego, por cada TID, puedes ver su mapa y localizar [stack:tid]\n");
    printf("    cat /proc/%d/task/<TID>/maps | grep -E '\\[stack|heap\\]'\n", getpid());
    printf("Pulsa ENTER aquí para continuar y hacer join...\n");
    fflush(stdout);
    getchar();

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    free(p_main);
    return 0;
}
