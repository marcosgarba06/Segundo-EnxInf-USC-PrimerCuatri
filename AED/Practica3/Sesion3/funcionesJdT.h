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


/*
* Lista todos los personajes de un tipo concreto de la base de datos del ABB
* @param arbol Arbol donde se encuentran los personajes
*/
void listadoTipo(TABB arbol);

//FUNCIONES DE GESTIÓN DE ARCHIVOS DE JUEGO DE TRONOS

/*
* Funcion que carga los personajes desde un archivo txt a la base de datos del ABB
* @param arbol Puntero al arbol donde se cargarán los personajes
* @param argc Numero de argumentos de la linea de comandos
* @param argv Vector de cadenas con los argumentos de la linea de comandos
*/
void cargar_archivo(TABB *arbol, int argc, char **argv);
/*
* Funcion que imprime en el archivo los persionajes de la base de datos del ABB
* @param arbol Arbol donde se encuentran los personajes
* @param argc Numero de argumentos de la linea de comandos
* @param argv Vector de cadenas con los argumentos de la linea de comandos
*/
void imprimirArchivo(TABB arbol, int argc, char **argv);

//FUNCION PARA BUSCAR EN EL ARBOL
/*
* Funcion que busca un personaje en la base de datos del ABB e imprime su informacion
* @param arbol Arbol donde se encuentran los personajes
*/
void buscarPersonaje(TABB arbol);


//FUNCION PARA MODIFICAR UN PERSONAJE
/*
* Funcion que modifica un personaje en la base de datos del ABB
* @param arbol Puntero al arbol donde se modificará el personaje
*/
void modificarPersonaje(TABB *arbol);

//FUNCION PARA BUSCAR LA FAMILIA MÁS NUMEROSA
/*
* Funcion que busca la familia más numerosa (suma de padres y hermanos) en la base de datos del ABB
* @param arbol Arbol donde se encuentran los personajes
*/
void familiaNumerosa(TABB arbol);

#endif /* FUNCIONESJDT_H */