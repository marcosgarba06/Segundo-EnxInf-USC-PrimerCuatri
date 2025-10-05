#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int var_global = 10; // Variable global
int main(int argc, char const *argv[])
{
    int local_var = 20; // Variable local
    int *dynamic_var = malloc(sizeof(int)); // Variable dinámica
    if (dynamic_var == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(dynamic_var);
        exit(1);
    }
    *dynamic_var = 30;
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // Proceso hijo
        printf("\nProceso hijo:\n");
        printf("PID: %d\n", getpid());
        printf("Valor var_global: %d    Dir memoria var_global: %p\n", var_global, &var_global);
        printf("Valor local_var: %d    Dir memoria local_var: %p\n", local_var, &local_var);
        printf("Valor dynamic_var: %d    Dir memoria dynamic_var: %p\n"  , *dynamic_var, dynamic_var);

        var_global += 5;
        local_var += 5;
        *dynamic_var += 5;

        printf("Valor var_global: %d    Dir memoria var_global: %p\n", var_global, &var_global);
        printf("Valor local_var: %d    Dir memoria local_var: %p\n", local_var, &local_var);
        printf("Valor dynamic_var: %d    Dir memoria dynamic_var: %p\n"  , *dynamic_var, dynamic_var);
    } else { // Proceso padre
        printf("\nProceso pai:\n");
        printf("Valor var_global: %d    Dir memoria var_global: %p\n", var_global, &var_global);
        printf("Valor local_var: %d    Dir memoria local_var: %p\n", local_var, &local_var);
        printf("Valor dynamic_var: %d    Dir memoria dynamic_var: %p\n"  , *dynamic_var, dynamic_var);

        var_global += 10;
        local_var += 10;
        *dynamic_var += 10;
        
        printf("Valor var_global: %d    Dir memoria var_global: %p\n", var_global, &var_global);
        printf("Valor local_var: %d    Dir memoria local_var: %p\n", local_var,&local_var);
        printf("Valor dynamic_var: %d   Dir memoria dynamic_var: %p\n"  , *dynamic_var, dynamic_var);
    } 
    return 0;
}
