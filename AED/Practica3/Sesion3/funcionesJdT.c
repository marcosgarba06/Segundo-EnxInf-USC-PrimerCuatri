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
    
    TIPOELEMENTOLISTA elemento;
    
    //printf("¿Desea añadir algun elemento a la lista %s? (s/n): ", name);
    //scanf(" %c", &opcion);
    

    while(1){
            
        printf("%s (o 'fin' para terminar): ", name);
        scanf(" %[^\n\r]", elemento.name);
        
        if (strcmp(elemento.name, "fin") == 0) {
            break; // Salir del bucle si el usuario ingresa "fin"
        }

        insertarElementoLista(lista,finLista(*lista), elemento);
        printf("%s '%s' añadido correctamente\n", name, elemento.name);
    }     
    
    printf("\n");
}


void anadirPersonaje(TABB *arbol) {
    //definicion variables
    TIPOELEMENTOABB nodo;

    
    printf("Nombre del personaje : ");
    scanf(" %[^\n\r]", nodo.name); 

    if(esMiembroAbb(*arbol,nodo)){
        printf("El personaje %s ya existe en la base de datos, no se puede anadir.\n", nodo.name);
        printf("\n");
        return;
    }

    //crear listas
    crearLista(&nodo.parents);
    crearLista(&nodo.siblings);
    crearLista(&nodo.killed);
    crearLista(&nodo.marriedEngaged);

    printf("Tipo de personaje (- en caso de desconocer): ");
    scanf(" %[^\n\r]", nodo.character_type);
    printf("\n");
    
    printf("Casa del personaje (- en caso de desconocer): ");
    scanf(" %[^\n\r]", nodo.house);
    printf("\n");
        

    printf("Es familia real (1 para si, 0 para no): ");
    scanf(" %hu", &nodo.real); //hu lee unsigned short
    printf("\n");

    printf("Descripcion del personaje (- en caso de desconocer): ");
    scanf(" %[^\n\r]", nodo.description);
    printf("\n");

    //procesar las listas
    hacerLista(&nodo.parents, "Padres");
    hacerLista(&nodo.siblings, "Hermanos");
    hacerLista(&nodo.killed, "Personas que ha asesinado");
    hacerLista(&nodo.marriedEngaged, "Personas que ha sido pareja");

    insertarElementoAbb(arbol, nodo);

    printf("Personaje '%s' anadido correctamente al arbol.\n", nodo.name);
        
}
    



void _imprimirLista(TLISTA lista){
    TPOSICION posActual, posFinal;
    TIPOELEMENTOLISTA elemento;
    int primero = 1;  // Variable flag para el primer elemento y no dejar una "," al final

    posFinal = finLista(lista);

    for(posActual = primeroLista(lista); posActual != posFinal; posActual = siguienteLista(lista, posActual)){
        recuperarElementoLista(lista, posActual, &elemento);
        if (!primero) {
            printf(",");
        }
        printf(" %s", elemento.name);
        primero = 0;
    }
}

void _imprimirPersonaje(TIPOELEMENTOABB personaje){
    printf("Nombre: %s\n", personaje.name);
    printf("Tipo: %s\n", personaje.character_type);
    printf("Casa: %s\n", personaje.house);
    printf("Familia real: %s\n", personaje.real ? "Si" : "No");

    //Lista de padres

    if (!esListaVacia(personaje.parents)) {
        printf("Padres:");
        _imprimirLista(personaje.parents);
        printf("\n");
    }
    
    /* Imprimir listas en cada caso comprobar que la lista no sea vacia, si lo es se imprime "-" 
    ya que es lo que indica que es un campo desconocido, si tienen elementos se imprime la lista
    con la funcion _imprimirLista creada anteiormente*/
    
    //Lista de hermanos

    if (! esListaVacia(personaje.siblings)) {
        printf("Hermanos:");
        _imprimirLista(personaje.siblings);
        printf("\n");
    }

    //Lista de personas que ha asesinado
    
    if (!esListaVacia(personaje.killed)) {
        printf("Personas que ha asesinado:");
        _imprimirLista(personaje.killed);
        printf("\n");
    }

    //Lista de personas que ha sido pareja
    
    if (esListaVacia(personaje.marriedEngaged)) {
        printf("Personas que ha sido pareja:");
        _imprimirLista(personaje.marriedEngaged);
        printf("\n");
    }

    printf("Descripcion: %s\n", personaje.description);
}


void listadoPersonajes(TABB arbol){
    TIPOELEMENTOABB elemen;
    if (esAbbVacio(arbol)) { //Comprobar que no es vacio el arbol
        //printf("El arbol esta vacio, no hay personajes que mostrar.\n");
        return;
    }else{
        //Usando recorrido InOrden (IRD)
        listadoPersonajes(izqAbb(arbol)); //Primero subarbol izquierdo recirsivamente
        
        leerElementoAbb(arbol, &elemen); //Elemento actual
        _imprimirPersonaje(elemen); 
        printf("\n");

        listadoPersonajes(derAbb(arbol)); //luego subarbol derecho
    }
}

void eliminarPersonaje(TABB *arbol){
    TIPOELEMENTOABB e;
    printf("Introduce el nombre del personaje a eliminar:");
    scanf(" %[^\n]", e.name);

    if(esMiembroAbb(*arbol,e)){
        buscarNodoAbb(*arbol,e.name,&e);

        suprimirElementoAbb(arbol, e);
        printf("El personaje %s, ha sido eliminado con éxito\n", e.name);
    }
    else{
        printf("El personaje que deseas eliminar no ha sido registrado en la base de datos.\n");
    }
}