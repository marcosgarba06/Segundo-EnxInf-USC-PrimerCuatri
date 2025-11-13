#ifndef TABLA_HASH_ENCADENAMIENTO_H
#define TABLA_HASH_ENCADENAMIENTO_H
//////////////////////////////////////////////////////////////////////////////////////////
//////MODIFICACIONES/////////////////////////////////////////////////////////////////////
//1. TAMAÑO DE LA TABLA (CONSTANTE N): probar con distintos tamaños
//2. FUNCIÓN InsertarHash(): convertirla en una función que devuelva 0 (no colisión) o 1 (colisión)
//3. FUNCIÓN BuscarHash(): añadir el parámetro nPasosExtraB
//4. FUNCIÓN FuncionHash(): probar con distintos valores de K en función hash tipo 3 
//////////////////////////////////////////////////////////////////////////////////////////

/* 
 * Tabla hash con encadenamiento
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

#define N 10000 /*DEFINIR NUMERO de entradas en la tabla.*/
//El factor de carga es L=n/N, donde n=datos (8.000)
//Se recomienda L<=0,5 para recolocación, es decir, N>=2n
//Se recomienda L<=0,75 para encadenamiento, es decir, N>=4n/3
//Debemos probar con N y el primo siguiente mayor que N

typedef TLISTA TablaHash[N];

/**
 * Inicializa cada elemento de la tabla a una lista vacía
 * @param t tabla hash.
 */
void InicializarTablaHash (TablaHash t);

/**
 * Destruye la lista que corresponde a cada elemento de la tabla
 * @param t tabla hash.
 */
void DestruirTablaHash (TablaHash t);

/**
 * Funciones hash: 3 tipos
 * @param cad clave para calcular la posición en la tabla hash.
 * @tipoFH=1-2-3, las tres funciones hash implementadas
 * @K es el factor de ponderación para las tipoFH=2-3
 * @return la posición de cad en la tabla hash
 */
int FuncionHash(char *cad, unsigned int tipoFH, unsigned int K);

/**
 * Buscar una clave en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @param clavebuscar: es la clave que buscamos
 * @param e es donde almacenamos el elemento encontrado
 * @param tipoFH es la función hash utilizada (1,2)
 * @param K es el factor de ponderación cuando tipoFH=2
 * @param nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la búsqueda tuvo éxito
 */
/////////////////////Hay que añadir nPasosExtraB//////////////////////////
int BuscarHash(TablaHash t, char *clavebuscar, TIPOELEMENTO *e, unsigned int tipoFH, unsigned int K, int* nPasosExtraB);

/**
 * Mira si clavebuscar está en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @param clavebuscar: es la clave que buscamos
 * @param tipoFH es la función hash utilizada (1,2)
 * @param K es el factor de ponderación cuando tipoFH=2
 * @nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la clave está en la tabla
 */
int EsMiembroHash (TablaHash t, char *clavebuscar, unsigned int tipoFH, unsigned int K, int* nPasosExtraB);

/**
 * Inserta un elemento en una tabla hash al principio de la lista
 * @param t es la tabla hash
 * @param elemento es el elemento que queremos guardar en la tabla
 * @param tipoFH es la función hash utilizada (1,2)
 * @param K es el factor de ponderación cuando tipoFH=2
 * @return devuelve 1 si se produce colisión y 0 en caso contrario
 */
///HACER QUE ESTA FUNCIÓN DEVUELVA UN ENTERO QUE INDIQUE SI HUBO O NO COLISIÓN
///POR TANTO NO SERÁ UNA FUNCIÓN void
int InsertarHash (TablaHash *t, TIPOELEMENTO elemento, unsigned int tipoFH, unsigned int K);

/**
 * Borra un elemento en una tabla hash
 * @param t es la tabla hash
 * @param claveborrar es el elemento que queremos borrar en la tabla
 * @param tipoFH es la función hash utilizada (1,2)
 * @param K es el factor de ponderación cuando tipoFH=2
 * @param nPasosExtraE son los pasos extra para encontrar el elemento a eliminar
*/
void BorrarHash (TablaHash *t, char *claveborrar,unsigned int tipoFH, unsigned int K, int* nPasosExtraE);
#endif	// TABLA_HASH_ENCADENAMIENTO_H

