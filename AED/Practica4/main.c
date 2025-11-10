#include <stdio.h>
#include <stdlib.h>
#include "JdTMap.h"
#define FILE_NAME "distancias.csv"
/*
 * Programa que muestra el uso del TAD grafo de números enteros
 */

int main(int argc, char** argv) {
    //Grafo de números enteros
    grafo G; //grafo
    char opcion;

    //Creo el grafo
    crear_grafo(&G);

    //Leo el archivo y creo el grafo
    leerArch(&G, FILE_NAME);

    do {
        printf("\n\na. Insertar nuevo vertice\n");
        printf("b. Eliminar vertice\n");
        printf("c. Crear arco\n");
        printf("d. Eliminar arco\n");
        printf("e. Buscar la ruta mas corta\n");
        printf("f. Buscar la ruta mas rapida\n");
        printf("g. Imprimir la infraestructura viaria de tiempo minimo\n");
        printf("i. Imprimir grafo\n");
        printf("s. Salir\n");

        printf("Opcion: ");
        scanf(" %c", &opcion); 

        switch (opcion) {
            case 'a':case'A':
                introducir_vertice(&G);
                break;
            case 'b':case 'B':
                eliminar_vertice(&G);
                break;
            case 'c': case 'C':
                nuevo_arco(&G);
                break;
            case 'd': case 'D':
                eliminar_arco(&G);
                break;
            case 'e': case 'E':
                buscar_ruta_mas_corta(&G);
                break;
            case 'f': case 'F':
                buscar_ruta_mas_rapida(&G);
                break;
            case 'g': case 'G':
                infraestructura_viaria_minima(&G);
                break;
            case 'i': case 'I':
                imprimir_grafo(G);
                break;
            case 's': case 'S':
                break;
            default:
                printf("Opción equivocada\n");
        }
    } while (opcion != 's');
    
    //Guardar el grafo en un archivo
    guardarArch(G, FILE_NAME); // CAMBIAR NOMBRE AL TERMINAR PRUEBAS
    
    //Al salir, liberamos la memoria del TAD, lo destruimos
    borrar_grafo(&G);

    return (EXIT_SUCCESS);
}