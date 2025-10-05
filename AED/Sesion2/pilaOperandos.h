#ifndef PILOPERANDOS_H
#define PILAOPERANDOS_H

#include "abin.h"  // Incluir la definicion de abin
//Interfaz TAD pilaOperandos
typedef void *pilaOperandos; /*tipo opaco*/

//CONTENIDO DE CADA ELEMENTO DE LA PILAOperandos
//MODIFICAR: PARA LA PILAOperandos DE OPERADORES: char
//MODIFICAR: PARA LA PILAOperandos DE OPERANDOS: abin
typedef abin tipoelemPilaOperandos;

//Funciones de creacion y destruccion
/**
 * Crea la pilaOperandos vacia. 
 * @param P Puntero a la pilaOperandos. Debe estar inicializada.
 */
void crearPilaOperandos(pilaOperandos *P);

/**
 * Destruye la pilaOperandos
 * @param P puntero a la pilaOperandos
 */
void destruirPilaOperandos(pilaOperandos *P);

//Funciones de informacion
/**
 * Comprueba si la pilaOperandos esta vacia
 * @param P pilaOperandos
 */
unsigned esVaciaPilaOperandos(pilaOperandos P);

/*
 * Recupera la informacion del tope de la pilaOperandos
 * @param P pilaOperandos
 * 
*/
tipoelemPilaOperandos topeOperandos(pilaOperandos P);

//Funciones de insercion/eliminacion
/**
 * Inserta un nuevo nodo en la pilaOperandos para el elemento E
 * en el tope de la pilaOperandos
 * @param P puntero a la pilaOperandos
 * @param E Informacion del nuevo nodo. 
 */
void pushOperandos(pilaOperandos *P, tipoelemPilaOperandos E);

/**
 * Suprime el elemento en el tope de la pilaOperandos
 * @param P puntero a la pilaOperandos
 */
void popOperandos(pilaOperandos *P);

#endif	// PILAOperandos_H