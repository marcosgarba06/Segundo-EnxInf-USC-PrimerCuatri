#ifndef FUNCIONESJDT_H
#define FUNCIONESJDT_H

#include "abb.h"

// FUNCIONES DE GESTIÓN DE PERSONAJES DE JUEGO DE TRONOS
/*
* Añade un personaje a la base de datos del ABB
* @param arbol Puntero al arbol donde se añadirá el personaje
*/
void anadirPersonaje(TABB *arbol);

/* 
* Lista todos los personajes de la base de datos del ABB
* @param arbol Arbol donde se encuentran los personajes
*/
void listadoPersonajes(TABB arbol);

/* 
* Elimina un personaje de la base de datos del ABB
* @param arbol Puntero al arbol donde se eliminará el personaje
*/
void eliminarPersonaje(TABB *arbol);

//FUNCIONES DE GESTIÓN DE ARCHIVOS DE JUEGO DE TRONOS
/*
* Funcion que imprime en el archivo los persionajes de la base de datos del ABB
* @param arbol Arbol donde se encuentran los personajes
* @param f Puntero al archivo donde se imprimirá la información
*/
void imprimirArchivo(TABB arbol, FILE *f);

#endif /* FUNCIONESJDT_H */