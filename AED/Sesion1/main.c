/* Practica 1: Prueba de manejo de Arboles Binarios  */
#include <stdlib.h>
#include <stdio.h>

#include "abin.h"
#include "recorridos.h"
#include "funcionesArboles.h"

int main(int argc, char** argv) {

    abin Arbol, aux;

    //Creo el Arbol
    crear(&Arbol);

    // Insercion de nodos en el arbol:
    printf("Inserción de la raíz\n");
    // El primero sera la raiz
    insizq(&Arbol, 10);
    //Imprimimos el árbol en preorden
    printf("\nPreOrden: ");preorden(Arbol);
    //Y mostramos su altura
    printf("\nAltura de arbol %d\n\n", AlturaArbol(Arbol));

    // Insertamos algunos elementos mas
    printf("Inserción de un segundo elemento");
    //Insertamos el nodo con valor 5 a la derecha del nodo raíz
    insder(&Arbol, 5);
    //Imprimimos el árbol en preorden
    printf("\nPreOrden: ");preorden(Arbol);
    //Y mostramos su altura
    printf("\nAltura de arbol %d\n\n", AlturaArbol(Arbol));
    
    printf("Insertamos más elementos\n");
    //Insertamos el nodo con valor 12 a la derecha de 5. 
    //Para eso tendremos que movernos a la derecha de la raíz
    aux = der(Arbol);
    insder(&aux, 12);
    //Insertamos nodo de valor 4 a la izquierda de nodo de valor 10(raíz)
    insizq(&Arbol, 4);
    //Insertamos un nodo a la izquierda de 5
    insizq(&aux, 7);
    //Insertamos un nodo a la izquierda de 4
    //Para eso tenemos que movernos a la izquierda de la raíz
    aux = izq(Arbol);
    insizq(&aux, 3);
    //Insertamos un nodo a la izquierda de 3
    //Para eso tenemos que ir dos veces a la izquierda de la raíz
    aux = izq(izq(Arbol));
    insizq(&aux, 6);
    //insertamos un nodo a la derecha de 3
    //Para eso podemos movernos a la derecha de aux o movernos desde raíz
    insder(&aux, 8);

    //Imprimimos la altura del árbol final
    printf("Altura de arbol: %d\n", AlturaArbol(Arbol));

    // Mostramos los elementos del arbol con distintos recorridos:
    printf("InOrden: ");    inorden(Arbol);
    printf("\nPreOrden: ");    preorden(Arbol);
    printf("\nPostOrden: ");    postorden(Arbol);
    printf("\nInorden no recursivo (usa pilas): "); inordenNR(Arbol);
    printf("\nRecorrido en anchura (usa colas): "); anchura(Arbol);

    //Por último, imprimimos el número de nodos del árbol
    printf("\nNumero de nodos: %d\n", NumeroNodos(Arbol));

    // Borramos algunos elementos:
    printf("\nVamos a eliminar el subárbol izquierdo de 5:\n");
    //Eliminamos el subarbol izquierdo de 5
    //Para ello nos situamos a la derecha de la raiz
    aux = der(Arbol);
    supizq(&aux);
    //E imprimimos en inorden, numero de nodos y altura del arbol
    printf("Inorden: ");     inorden(Arbol);
    printf("\nN nodos: %d", NumeroNodos(Arbol));
    printf("\nAltura de arbol: %d", AlturaArbol(Arbol));

    printf("\n\nVamos a eliminar el subárbol izquierdo de la raíz\n");
    //Eliminamos el subarbol izquierdo del nodo raiz
    supizq(&Arbol);

    //E imprimimos en inorden, el numero de nodos y la altura del arbol
    printf("Inorden: ");   inorden(Arbol);
    printf("\nN nodos: %d\n", NumeroNodos(Arbol));
    printf("\nAltura de arbol: %d\n", AlturaArbol(Arbol));

    // Liberamos memoria asociada al arbol:
    destruir(&Arbol);

    return (EXIT_SUCCESS);
}
