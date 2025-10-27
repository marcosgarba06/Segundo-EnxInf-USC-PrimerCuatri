#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/////////////////////////////////////////////////////////// TIPOS DE DATOS

// Estructura privada
struct tipografo {
    int N; //número de vértices del grafo
    tipovertice VERTICES[MAXVERTICES]; //vector de vértices
    tipoarco A[MAXVERTICES][MAXVERTICES]; //matriz de adyacencia cambiada de int a tipoarco para poder almacenar pesos y tipos de conexión
};

//////////////////////////////////////////////////////////////// FUNCIONES

//HAY QUE MODIFICAR ESTA FUNCIÓN SI SE CAMBIA EL TIPO DE DATO tipovertice
/* 
 * Esta función devuelve 0 si los dos nodos son iguales
 * y 1 en otro caso.
 */
int _comparar_vertices(tipovertice V1, tipovertice V2){
	return (strcmp(V1.name, V2.name) == 0) ? 0 : 1;
}


//Creación del grafo con 0 nodos
void crear_grafo(grafo *G) {
    *G = (struct tipografo*) malloc(sizeof (struct tipografo));
    (*G)->N = 0;
}

//Devuelve la posición del vértice Vert en el vector VERTICES del grafo G
//Si devuelve -1 es porque no encontró el vértice
int posicion(grafo G, tipovertice V) {
    int contador = 0;
    //comparo V con todos los vertices almacenados en VERTICES 
    while (contador < G->N) {
        //if (G->VERTICES[contador]==V)  //encontré la posicion de V
		if (_comparar_vertices(G->VERTICES[contador], V) == 0){
            return contador; 
        }
        contador++;
    }
    return -1;
}

//Devuelve 1 si el grafo G existe y 0 en caso contrario
int existe(grafo G) {
    return (G != NULL);
}

//Devuelve 1 si el vértice Vert existe en el grafo G
int existe_vertice(grafo G, tipovertice V) {
    return (posicion(G, V) >= 0);
}

//Inserta un vértice en el grafo, devuelve -1 si no ha podido insertarlo por estar el grafo lleno
int insertar_vertice(grafo *G, tipovertice Vert) {
    int i;
    if ((*G)->N == MAXVERTICES) {
    	// Se ha llegado al maximo numero de vertices
    	return -1;
    }
   
    (*G)->N++;
    (*G)->VERTICES[((*G)->N) - 1] = Vert;

    //Al inicializar el nuevo vértice, ponemos a -1 la distancia y '\0' el tipo de conexión
    for (i = 0; i < (*G)->N; i++) {
        (*G)->A[i][((*G)->N) - 1].distancia = 0.0;
        (*G)->A[i][((*G)->N) - 1].tipoconexion = 0;
        (*G)->A[((*G)->N) - 1][i].distancia = 0.0;
        (*G)->A[((*G)->N) - 1][i].tipoconexion = 0;
    }
	return (*G)->N-1;
}

//Borra un vertice del grafo
void borrar_vertice(grafo *G, tipovertice Vert) {
    int F, C, P, N = (*G)->N;
    P = posicion(*G, Vert);
    if(P == -1){
    	return;
    }
    //if (P >= 0 && P < (*G)->N) {
    for (F = P; F < N - 1; F++){
        (*G)->VERTICES[F] = (*G)->VERTICES[F + 1];
	}
    for (C = P; C < N - 1; C++){
        for (F = 0; F < N; F++){
            (*G)->A[F][C] = (*G)->A[F][C + 1];
        }
	}
    for (F = P; F < N - 1; F++){
        for (C = 0; C < N; C++){
            (*G)->A[F][C] = (*G)->A[F + 1][C];
        }
	}
    (*G)->N--;    
}

//Crea el arco de relación entre VERTICES(pos1) y VERTICES(pos2)
void crear_arco(grafo *G, int pos1, int pos2, float distancia, char tipo) {
    //(*G)->A[pos1][pos2] = 1;

    if (tipo != 't' && tipo != 'm' && tipo != 'T' && tipo != 'M') return; //si el tipo no es válido, no hacemos nada
    // Normalizamos tipo a minúscula
    if (tipo == 'T') tipo = 't';
    if (tipo == 'M') tipo = 'm';

    (*G)->A[pos1][pos2].distancia = distancia;
    (*G)->A[pos1][pos2].tipoconexion = tipo;

    //Al ser unn grafo no dirigido, el arco es bidireccional, es decir hay arco de pos1 a pos2 y de pos2 a pos1, por lo que 
    //la matriz es simétrica y se acrtualizan ambas posiciones

    (*G)->A[pos2][pos1].distancia = distancia;
    (*G)->A[pos2][pos1].tipoconexion = tipo;
}

//Borra el arco de relación entre VERTICES(pos1) y VERTICES(pos2)
void borrar_arco(grafo *G, int pos1, int pos2) {
    //(*G)->A[pos1][pos2] = 0;
    (*G)->A[pos1][pos2].distancia = 0.0;
    (*G)->A[pos1][pos2].tipoconexion = 0;

    //Al ser no dirigido se borran los arcos en ambas direcciones
    (*G)->A[pos2][pos1].distancia = 0.0;
    (*G)->A[pos2][pos1].tipoconexion = 0;


}

//Devuelve 1 si VERTICES(pos1) y VERTICES(pos2) son vértices adyacentes
int son_adyacentes(grafo G, int pos1, int pos2) {
    return (G->A[pos1][pos2].tipoconexion != 0);
}

// Devuelve la distancia entre dos VERTICES
float distancia(grafo G, int pos1, int pos2) {
    return G->A[pos1][pos2].distancia;
}

// Devuelve el tipo de conexión entre dos VERTICES
char tipoconexion(grafo G, int pos1, int pos2) {
    return G->A[pos1][pos2].tipoconexion;
}

//Destruye el grafo
void borrar_grafo(grafo *G) {
    free(*G);
    *G = NULL;
}

//Devuelve el número de vértices del grafo G
int num_vertices(grafo G) {
    return G->N;
}

//Devuelve el vector de vértices VERTICES del grafo G
tipovertice* array_vertices(grafo G) {
    return G->VERTICES;
}

