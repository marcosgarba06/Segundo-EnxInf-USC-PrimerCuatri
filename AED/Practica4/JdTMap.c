#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

//Opción a del menú, introducir un vertice en el grafo

void introducir_vertice(grafo *G) {
    tipovertice v1;
    
    //Para introducir un vértice pedimos la ciudad y la región en la que está ya que tipovertice es una estructura con esos dos campos
    printf("Introduce nombre de la ciudad: ");
    scanf(" %[^\n]", v1.name);  

    printf("Introduce región: ");
    scanf(" %[^\n]", v1.region);
    
    //Comprobar si el vertice ya esta en el grafo
    if (existe_vertice(*G, v1))
        printf("Ese vertice ya esta en el grafo\n");
    else
        insertar_vertice(G, v1);
}

//Opción b del menú, eliminar un vértice del grafo

void eliminar_vertice(grafo *G) {
    tipovertice v1;
    
    //Para eliminar un vértice pedimos la ciudad y la región en la que está ya que tipovertice es una estructura con esos dos campos
    printf("Introduce nombre de la ciudad: ");
    scanf(" %[^\n]", v1.name);

    printf("Introduce región: ");
    scanf(" %[^\n]", v1.region);

    //Comprobar si el vertice existe en el grafo, si no existe, da un mensaje de error    
    if (existe_vertice(*G, v1))
        borrar_vertice(G, v1);
    else
        printf("Ese vertice no existe en el grafo\n");
}

//Opción c del menú, crear una relación entre dos vértices

void nuevo_arco(grafo *G) {
    tipovertice v1, v2;
    tipoarco arco;

    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1-->vertice2\n");
    
    //Vértice origen del arco
    printf("Introduce nombre de la ciudad origen: ");
    scanf(" %[^\n]", v1.name);
    
    if (!existe_vertice(*G, v1)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v1.name, v1.region);
        return;
    }

    //Vértice destino del arco
    printf("Introduce nombre de la ciudad destino: ");
    scanf(" %[^\n]", v2.name);
    
    /*
    printf("Introduce región destino: ");
    scanf(" %[^\n]", v2.region);*/
    
    if (!existe_vertice(*G, v2)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v2.name, v2.region);
        return;
    }

    //Datos del arco
    printf("Distancia (km): ");
    scanf("%f", &arco.distancia);

    printf("Tipo de conexion ('t' tierra, 'm' mar): ");
    scanf(" %c", &arco.tipoconexion);

    if (arco.tipoconexion != 't' && arco.tipoconexion != 'm' && arco.tipoconexion != 'T' && arco.tipoconexion != 'M') {
        printf("La conexión entre 2 ciudades solo puede ser por mar o por tiera.\n");
        return;
    }
    //Creación del arco
    if (!son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        crear_arco(G, posicion(*G, v1), posicion(*G, v2), arco.distancia, arco.tipoconexion);
    else
        printf("Ya hay conexión entre estas ciudades\n");
}

//Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G) {
    tipovertice v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
   
    //Vértice origen del arco
    printf("Introduce nombre de la ciudad origen: ");
    scanf(" %[^\n]", v1.name);

    /*
    printf("Introduce región origen: ");
    scanf(" %[^\n]", v1.region);*/
    
    if (!existe_vertice(*G, v1)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v1.name, v1.region);
        return;
    }

    //Vértice destino del arco
    printf("Introduce nombre de la ciudad destino: ");
    scanf(" %[^\n]", v2.name);
    printf("Introduce región destino: ");
    scanf(" %[^\n]", v2.region);
    
    if (!existe_vertice(*G, v2)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v2.name, v2.region);
        return;
    }

    //Eliminación del arco
    if (son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        borrar_arco(G, posicion(*G, v1), posicion(*G, v2));
    else
        printf("No hay conexión entre estas ciudades\n");
}

//Opción i del menú, imprimir el grafo
//Función que imprime el grafo utilizando num_vertices para saber cuántos vértices tiene
//y array_vertices para recuperar el vector de vértices y recorrerlo

void imprimir_grafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo
    float dist; //distancia entre vértices
    char tipo; //tipo de conexión entre vértices

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;
    printf("El grafo actual es:\n");
    for (i = 0; i < N; i++) {
        //Imprimo el vértice
        printf("Vertice(%d): %s (Region: %s)\n", i, VECTOR[i].name, VECTOR[i].region);
        //Chequeo sus arcos
        for (j = 0; j < N; j++)
            //Si son adyacentes imprimo la relación
            if (son_adyacentes(G, i, j))
            {
                //Calcular distancia y tipo de conexión del arco
                dist = distancia(G, i, j);
                tipo = tipoconexion(G, i, j);

                printf("\t %s (Region: %s) --> %s (Region: %s) | Distancia: %f km | Tipo de conexion: %c\n", 
                VECTOR[i].name, VECTOR[i].region, VECTOR[j].name, VECTOR[j].region, dist, tipo);
            }
    }
}

