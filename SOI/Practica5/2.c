//Ejercicio 2
//Marcos García Blanco e Inés Landeira Rodríguez
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h> 

void f1(int x){
    int local_f1 = x + 1;
    printf("[f1]  Direccion del codigo f1 = %p\n", (void*)f1);
    printf("[f1]  Direccion del parametro x = %p\n", (void*)&x);
    printf("[f1]  Direccion de la variable local_f1 = %p\n", (void*)&local_f1);
}
void f2(int y){
    int local_f2= y +1;
    printf("[f2] Direccion del codigo f2  = %p\n", (void*)f2);
    printf("[f2] Direccion del parametro y = %p\n", (void*)&y);
    printf("[f2] Direccion de la variable local_f2 = %p\n", (void*)&local_f2);
}

int main(){ 
    int PID=getpid();
    printf("PID = %d\n", PID);
    f1(123);
    f2(456);
    printf("Consulta el mapa de memoria tras ejecutar las funciones en /proc/%d/maps. \n", PID);
    printf("Pulsa ENTER para salir...\n");
    scanf("%*c");
    return 0;
}