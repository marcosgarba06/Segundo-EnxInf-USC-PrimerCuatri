//Marcos García Blanco e Inés Landeira Rodríguez

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

static void pausa(const char *msg, pid_t pid) {
    printf("\n%s  (PID=%d)\n", msg, pid);
    printf("Abre otra terminal y mira:  cat /proc/%d/maps | less\n", pid);
    printf("Pulsa ENTER para continuar...\n");
    fflush(stdout);
    getchar();
}

static int es_nueva_imagen(int argc, char **argv) {
    return (argc >= 2 && strcmp(argv[1], "EXEC") == 0);
}

int main(int argc, char **argv) {
    if (es_nueva_imagen(argc, argv)) {
        // ========= MODO "NUEVA IMAGEN" (tras execv) =========
        pid_t pid = getpid();
        printf("\n[NUEVA IMAGEN] execv cargado. PID=%d\n", pid);
        printf("Este proceso es una imagen nueva (heap/pila re-inicializados).\n");
        pausa("[NUEVA IMAGEN] Mapa ACTUAL (debe ser 'limpio' respecto al hijo pre-exec)", pid);
        return 0;
    }

    // ========= MODO NORMAL (PADRE) =========
    pid_t padre = getpid();
    printf("[PADRE] PID=%d\n", padre);
    pausa("[PADRE] Mapa ANTES del fork()", padre);

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        // ========= HIJO =========
        pid_t hijo = getpid();
        printf("\n[HIJO] PID=%d (recién creado tras fork)\n", hijo);
        pausa("[HIJO] Mapa INICIAL (debe parecerse al del padre: copy-on-write)", hijo);

        // malloc en el hijo
        void *p = malloc(1024 * 1024); // 1 MiB
        if (!p) { perror("malloc"); exit(1); }
        printf("[HIJO] malloc(1MiB) -> %p\n", p);
        pausa("[HIJO] Mapa TRAS el malloc (observa [heap] o nuevas anonimas rw-p)", hijo);

        // Cambiar imagen con execv al propio binario en modo "EXEC"
        printf("[HIJO] Llamando a execv(\"%s\", {\"%s\",\"EXEC\"})...\n", argv[0], argv[0]);
        fflush(stdout);
        char *const nargv[] = { argv[0], (char*)"EXEC", NULL };
        execv(argv[0], nargv);

        // Si llegamos aquí, execv falló
        perror("execv");
        exit(1);
    } else {
        // ========= PADRE =========
        int status = 0;
        printf("\n[PADRE] Esperando a que termine el hijo (PID=%d)...\n", pid);
        waitpid(pid, &status, 0);
        printf("[PADRE] Hijo terminó (status=%d).\n", status);
        pausa("[PADRE] Mapa FINAL del padre (no cambia por malloc/exec del hijo)", padre);
    }
    return 0;
}
