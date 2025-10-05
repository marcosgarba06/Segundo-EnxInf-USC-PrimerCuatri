/* Practica 1: Prueba de manejo de Arboles Binarios  */
#include <stdlib.h>
#include <stdio.h>

#include "abin.h"
#include "recorridos.h"
#include "funcionesArboles.h"

int main(int argc, char** argv) {

    //Declaracion variables usadas en el main
    abin Arbol, aux;
    char nodoBuscar;

    //Creo el Arbol
    crear(&Arbol);

    // Insercion de nodos en el arbol:    
    // El primero sera la raiz
    insizq(&Arbol, 'J');

    // Insertamos el resto de elementos
    //Insertamos el nodo M a la derecha de la raiz
    insder(&Arbol, 'M'); 
    
    aux = der(Arbol); //Nos situamos a la derecha del arbol
    insder(&aux, 'W'); //Insertamos a la derecha el nodo W
    insizq(&aux, 'K'); //Insertamos a la izquierda el nodo K

    insizq(&Arbol, 'F'); //Insertamos a la izquierda del nodo raiz el nodo F
    
    //Nos situamos en el nodo F
    aux = izq(Arbol);
    insizq(&aux, 'D');
    insder(&aux, 'G');

    //Nos situamos en el nodo D
    aux = izq(izq(Arbol)); 
    insizq(&aux, 'B');
    insder(&aux, 'E');

    //Nos situamos en el nodo G
    aux =  der(izq(Arbol));
    insder(&aux, 'H');

    printf("Arbol creado \n");

    //Imprimimos la altura del árbol final
    printf("Altura de arbol: %d\n", AlturaArbol(Arbol));

    //Imprimimos con Inorden no recursivo
    printf("\nRecorrido inorden no recursivo (usa pilas): "); inordenNR(Arbol);

    //Eliminamos el subarbol derecho del nodo F forma 1:
    /*aux = izq(Arbol);
    supder(&aux);
    printf("\nArbol derecho de F eliminado");
    */

    //Eliminamos el subarbol derecho con funcion buscar
    aux = NULL; //Ponemos el aux a null para que no haya errores en la opcion buscar
    
    printf("\nNodo a buscar: "); //Pregunta al usuario nodo a buscar
    nodoBuscar = getchar(); //lee solo un caracter

    buscar(Arbol, nodoBuscar, &aux); //busca el nodo con la funcion recursiva
   
    supder(&aux); //Suprime arbol derecho del nodo a buscar, corregir por si da un NULL
    printf("\nArbol derecho de %c eliminado", nodoBuscar);
    

    //Se imprime el recorrido IRD, la altura y el numero de nodos despues de eliminar el nodo F
    printf("\nRecorrido InOrden (IRD): ");     inorden(Arbol);
    printf("\nAltura de arbol: %d", AlturaArbol(Arbol));
    printf("\nNumero de nodos: %d\n", NumeroNodos(Arbol));

    // Liberamos memoria asociada al arbol:
    destruir(&Arbol);

    return (EXIT_SUCCESS);
}
