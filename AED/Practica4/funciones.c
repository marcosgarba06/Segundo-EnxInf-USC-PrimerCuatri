#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

//Opción a del menú, introducir un vertice en el grafo

void introducir_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce vertice: ");
    scanf("%d", &v1);
    if (existe_vertice(*G, v1))
        printf("Ese vertice ya esta en el grafo\n");
    else
        insertar_vertice(G, v1);
}
//Opción b del menú, eliminar un vértice del grafo

void eliminar_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce vertice: ");
    scanf("%d", &v1);
    if (existe_vertice(*G, v1))
        borrar_vertice(G, v1);
    else
        printf("Ese vertice no existe en el grafo\n");
}

//Opción c del menú, crear una relación entre dos vértices

void nuevo_arco(grafo *G) {
    tipovertice v1, v2;
    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%d", &v1);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %d no existe en el grafo\n", v1);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%d", &v2);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %d no existe en el grafo\n", v2);
        return;
    }
    //Creación del arco
    if (!son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        crear_arco(G, posicion(*G, v1), posicion(*G, v2));
}

//Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G) {
    tipovertice v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf("%d", &v1);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %d no existe en el grafo\n", v1);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf("%d", &v2);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %d no existe en el grafo\n", v2);
        return;
    }
    //Eliminación del arco
    if (son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        borrar_arco(G, posicion(*G, v1), posicion(*G, v2));
}

//Opción i del menú, imprimir el grafo
//Función que imprime el grafo utilizando num_vertices para saber cuántos vértices tiene
//y array_vertices para recuperar el vector de vértices y recorrerlo

void imprimir_grafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;
    printf("El grafo actual es:\n");
    for (i = 0; i < N; i++) {
        //Imprimo el vértice
        printf("Vertice(%d): %d\n", i, VECTOR[i]);
        //Chequeo sus arcos
        for (j = 0; j < N; j++)
            if (son_adyacentes(G, i, j))
                printf("\t%d-->%d\n", VECTOR[i], VECTOR[j]);
    }
}

