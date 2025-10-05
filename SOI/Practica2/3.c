// Código feito for Nicolás S. Gómez Forján e Marcos García Blanco

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char car = 'p'; //variable global

int main()
{
    float pi = 3.141592f; //variable local
    int* var = (int*)malloc(sizeof(int)); //variable dinamica
    if (var == NULL) {
        fprintf(stderr, "Error na asignación de memoria\n");
        exit(1);
    }
    *var = 13;
    printf("Inicio do programa pai\n");
    printf("Pi é igual a %f e está almacenado en %p, car é igual a %c e está almacenado en %p e var é igual a %d e está almacenado en %p\n\n", pi, &pi, car, &car, *var, var);
    char texto[64];

    int fid = fork(); 
    if (fid < 0)
    {
        printf("Erro ao crear un novo proceso\n");
    }
    else if (fid == 0)
    {
        car = 'v';
        pi = 4;
        *var = 12;
        printf("Este proceso é o fillo con PID %d, PID do pai %d, UID %d, UID efectiva %d e GUID %d\n", getpid(), getppid(), getuid(), getegid(), getgid());
        printf("O ambiente é %s\n", getenv("PATH"));

        printf("Pi é igual a %f e está almacenado en %p, car é igual a %c e está almacenado en %p e var é igual a %d e está almacenado en %p\n\n", pi, &pi, car, &car, *var, var);
        
        //printf("Escribe por teclado (fillo): ");
        scanf("%s", texto);
        printf("O fillo reciviu %s do teclado\n", texto);
        
        free(var);
    }
    else
    {
        car = 'a';
        pi = 5;
        *var = 14;
        printf("Este proceso é o pai con PID %d, PID do pai %d, UID %d, UID efectiva %d e GUID %d\n", getpid(), getppid(), getuid(), getegid(), getgid()); //imprime os datos do proceso
        printf("O ambiente é %s\n", getenv("PATH")); //imprime a variable de entorno PATH

        //Imprimir os valores e as direccións das variables
        printf("Pi é igual a %f e está almacenado en %p, car é igual a %c e está almacenado en %p e var é igual a %d e está almacenado en %p\n\n", pi, &pi, car, &car, *var, var); 
        
        //printf("Escribe por teclado (pai): ");
        scanf("%s", texto);
        printf("O pai reciviu %s do teclado\n", texto);
        
        free(var);
    }    
}

