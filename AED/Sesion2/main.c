/*
    Introduce la expresion aritmetica infija: x*5/(A+B)

    La expresion prefija correspondiente es:
    / * x 5 + A B

    La expresion postfija correspondiente es:
    x 5 * A B + / 
*/

#include <stdio.h>
#include <stdlib.h>

#include "abin.h"
#include "FuncionesExpresion.h"
#include "recorridos.h"

int main(int argc, char const *argv[]){
    char mensaje[100]; //cadena para almacenar el mensaje
    abin arbolExp; //varu
    
    printf("Introduce la expresión:");
    scanf("%s",mensaje);

    arbolExp = arbolExpresion(mensaje);  //crea el arbol a partir de la expresion

    preorden(arbolExp); //imprime en notacion prefija
    printf("\n"); //para que haya separacion entre las lineas

    postorden(arbolExp); //imprime en notacion postfija
    printf("\n");

    destruir(&arbolExp); //destruye el arbol antes de salir del programa

    return 0;
}
