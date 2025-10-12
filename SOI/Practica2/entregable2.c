#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
// Codigo realizado por Marcos García Blanco y Nicolás S. Gómez Forján

int main(int argc, char* argv[])
{
    int pid[2];

    pid[0] = fork(); //crear fillo1 (zombie)

    // Se estamos no primeiro fillo
    if (pid[0] == 0)
    {
        //matar fillo1 para facelo zombie
        printf("Proceso fillo morrendo...\n");
        exit(128);
    }
    else
    {
        sleep(10); //para que tempo a facer ps e ver que pasa
        int status;

        wait(&status); //esperar polo fillo1

        printf("O proceso zombi saiu con exit(%d)\n", WEXITSTATUS(status)); //imprimir o exit do fillo1

        pid[1] = fork(); //crear fillo2 (orfo)

        if (pid[1] == 0)
        {
            printf("Proceso fillo indo a orfo...\n"); 
            sleep(5);
            execvp("/usr/bin/man", argv); //cambiar a imaxe do proceso polo comando man
            printf("O fillo orfo fixo man\n"); //non se deberia ver nunca
        }
        else
        {
            printf("Proceso pai morrendo\n");
            return 0;
        }
    }

    return 1;
}