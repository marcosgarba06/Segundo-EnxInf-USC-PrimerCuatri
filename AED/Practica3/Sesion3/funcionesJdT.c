#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionesJdT.h"
#include "abb.h"
#include "lista.h"
#define NAME_LENGTH 50
//Opcion A/a Anadir personaje
//Preguntar por el nombre, tipo, casa, es familia real, y varias listas: padres, hermanos
//personas que ha asesinado, personas que ha sido pareja
//Para cada lista se preguntará el nombre del personaje, finalizando el proceso de 
//rellenarla cuando el nombre del personaje sea "fin". Cuando exista algún campo desconocido, 
//el usuario tecleará el  carácter ’-’.

void hacerLista(TLISTA *lista, char *name){
    char cad[NAME_LENGTH];
    TIPOELEMENTOLISTA elemento;
    char opcion;
    
    printf("¿Desea añadir algun elemento a la lista %s? (s/n): ", name);
    scanf(" %c", &opcion);
    
    if (opcion == 's' || opcion == 'S') {
        while(1){
            
            printf("%s (o 'fin' para terminar): ", name);
            scanf(" %[^\n\r]", cad);
            
            if (strcmp(cad, "fin") == 0) {
                break; // Salir del bucle si el usuario ingresa "fin"
            }
            strncpy(elemento.name, cad, NAME_LENGTH);
            insertarElementoLista(lista,primeroLista(*lista), elemento);
            printf("%s '%s' añadido correctamente\n", name, elemento.name);
            
        }
            
    }
    printf("\n");
}


void anadirPersonaje(TABB *arbol) {
    //definicion variables
    TIPOELEMENTOABB nodo;

    while (1)
    {
        //crear listas
        crearLista(&nodo.parents);
        crearLista(&nodo.siblings);
        crearLista(&nodo.killed);
        crearLista(&nodo.marriedEngaged);

        getchar(); // Limpiar el buffer de entrada

        printf("Nombre del personaje (o 'fin' para terminar): ");
        scanf(" %[^\n\r]", nodo.name);

        getchar(); // Limpiar el buffer de entrada

        if (strcmp(nodo.name, "fin") == 0) {
            // Destruir las listas para liberar memoria
            destruirLista(&nodo.parents);
            destruirLista(&nodo.siblings);
            destruirLista(&nodo.killed);
            destruirLista(&nodo.marriedEngaged);
            break; // Salir del bucle si el usuario ingresa "fin"
        }

        printf("Tipo de personaje: ");
        scanf(" %[^\n\r]", nodo.character_type);
        printf("\n");
       
        printf("Casa del personaje: ");
        scanf(" %[^\n\r]", nodo.house);
        printf("\n");
        

        printf("Es familia real (1 para si, 0 para no): ");
        scanf(" %hu", &nodo.real); //hu lee unsigned short
        getchar(); // Limpiar el buffer de entrada
        printf("\n");

        printf("Descripcion del personaje: ");
        scanf(" %[^\n\r]", nodo.description);
        printf("\n");

        //procesar las listas
        hacerLista(&nodo.parents, "Padres");
        hacerLista(&nodo.siblings, "Hermanos");
        hacerLista(&nodo.killed, "Personas que ha asesinado");
        hacerLista(&nodo.marriedEngaged, "Personas que ha sido pareja");

        insertarElementoAbb(arbol, nodo);

         // Destruir las listas para liberar memoria
        destruirLista(&nodo.parents);
        destruirLista(&nodo.siblings);
        destruirLista(&nodo.killed);
        destruirLista(&nodo.marriedEngaged);

        printf("Personaje '%s' anadido correctamente al arbol.\n", nodo.name);
        
    }
    
    printf("Finalizando anadir personaje...\n");

}