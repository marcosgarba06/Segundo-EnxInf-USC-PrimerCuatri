//Codigo por Marcos García Blanco y Nicolas Santiago Gómez Forján

#include <stdio.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        printf("Proceso hijo.\n");
        printf("PID: %d\n", getpid()); //Devuelve el PID (Process ID) del proceso actual.
        printf("PID de mi padre: %d\n", getppid()); //Devuelve el PID (Process ID) del proceso padre.
        printf("UID real: %d\n",  getuid()); //Devuelve el UID real (User ID) del usuario que ejecuta el proceso.
        printf("UID efectivo: %d\n", geteuid()); //Devuelve el UID efectivo (User ID) del usuario que ejecuta el proceso.
        printf("GID real: %d \n",getgid()); //Devuelve el GID real (Group ID) del grupo primario del usuario que ejecuta el proceso.
        printf("Variable de entorno PATH: %s\n", getenv("PATH")); //getenv nos devuelve la variable de entorno PATH
    } else {
        printf("Proceso padre.\n");
        printf("PID: %d\n", getpid());
        printf("PID de mi padre: %d\n", getppid());
        printf("UID real: %d\n", getuid());
        printf("UID efectivo: %d\n",geteuid());
        printf("GID: %d \n",getgid());
        printf("Variable de entorno Home: %s\n", getenv("HOME"));
    }  
    return 0;
}
