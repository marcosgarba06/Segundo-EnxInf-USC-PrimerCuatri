#ifndef TABLA_HASH_RECOLOCACION_H
#define TABLA_HASH_RECOLOCACION_H
//////////////////////////////////////////////////////////////////////////////////////////
//////MODIFICACIONES/////////////////////////////////////////////////////////////////////
//1. TAMAÑO DE LA TABLA (CONSTANTE N): probar con distintos tamaños
//2. FUNCIÓN InsertarHash(): convertirla en una función que devuelva 0 (no colisión) o 1 (colisión)
//   y añadir parámetro nPasosExtraI
//3. FUNCIÓN BuscarHash(): añadir el parámetro nPasosExtraB
//4. FUNCIÓN FuncionHash(): probar con distintos valores de K en función hash tipo 3 
//////////////////////////////////////////////////////////////////////////////////////////

/* 
 * Tabla hash con recolocación
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 16003 /*DEFINIR NUMERO de entradas en la tabla.*/
//El factor de carga es L=n/N, donde n=datos (10.000)
//Se recomienda L<=0,5 para recolocación, es decir, N>=2n
//Se recomienda L<=0,75 para encadenamiento, es decir, N>=4n/3
//Debemos probar con N y el primo siguiente mayor que N

///////////CONSTANTES PARA MARCAR LAS CELDAS BORRADAS O VACÍAS
#define VACIO '\0'
#define BORRADO ' '
/////////CONSTANTES PARA DEFINIR LOS CAMPOS DEL TIPOELEMENTO
#define L 15

typedef struct{
    char nombre[3*L];
    char alias[L];
    char correo[3*L];
}TIPOELEMENTO;

//DEFINICIÓN DEL TAD TablaHash
typedef TIPOELEMENTO TablaHash[N];

/**
 * Inicializa cada elemento de la tabla a un contenido vacío (constante VACIO)
 * @param t tabla hash.
 */
void InicializarTablaHash(TablaHash t);

/**
 * Funciones hash: 3 tipos
 * @param cad clave para calcular la posición en la tabla hash.
 * @tipoFH=1-2-3, las tres funciones hash implementadas
 * @K es el factor de ponderación para las tipoFH=2-3
 * @return la posición de cad en la tabla hash
 */
int FuncionHash(char *cad, unsigned int tipoFH, unsigned int K);

/**
 * Mira si clavebuscar está en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @clavebuscar: es la clave que buscamos
 * @tipoFH es la función hash utilizada (1,2,3)
 * @K es el factor de ponderación para las tipoFH=2-3
 * @tipoR es la recolocación utilizada (1=lineal, usa el parámetro @a, 2=cuadrática)
 * @a es el parámetro a de la recolocación lineal
 * @nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la clave está en la tabla
 */
int EsMiembroHash(TablaHash t, char *clavebuscar, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraI);

/**
 * Buscar una clave en la tabla hash
 * @param t es la tabla hash en la que buscamos la clave
 * @clavebuscar: es la clave que buscamos
 * @e es donde almacenamos el elemento encontrado
 * @tipoFH es la función hash utilizada (1,2,3)
 * @K es el factor de ponderación para las tipoFH=2-3
 * @tipoR es la recolocación utilizada (1=lineal, usa el parámetro @a, 2=cuadrática)
 * @a es el parámetro a de la recolocación lineal
 * @nPasosExtraB son los pasos que tengo que realizar en cada lista hasta encontrar e
 * @return si la búsqueda tuvo éxito
 */
/////////////////////Hay que añadir nPasosExtraB como parámetro por referencia
int BuscarHash(TablaHash t, char *clavebuscar, TIPOELEMENTO *e,unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraB);

/**
 * Inserta un elemento en una tabla hash al principio de la lista
 * @param t es la tabla hash
 * @elemento es el elemento que queremos guardar en la tabla
 * @tipoFH es la función hash utilizada (1,2,3)
 * @K es el factor de ponderación para las tipoFH=2-3
 * @tipoR es la recolocación utilizada (1=lineal, usa el parámetro @a, 2=cuadrática)
 * @a es el parámetro a de la recolocación lineal
 * @nPasosExtraI son los pasos que tengo que realizar hasta encontrar una posición libre
 * @return devuelve 1 si se produce colisión y 0 en caso contrario
 */
///HACER QUE ESTA FUNCIÓN DEVUELVA UN ENTERO QUE INDIQUE SI HUBO O NO COLISIÓN
///POR TANTO NO SERÁ UNA FUNCIÓN void
int InsertarHash(TablaHash t, TIPOELEMENTO e, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraI);

/**
 * Borra un elemento en una tabla hash
 * @param t es la tabla hash
 * @claveborrar es el elemento que queremos borrar en la tabla
 * @tipoFH es la función hash utilizada (1,2,3)
 * @K es el factor de ponderación para las tipoFH=2-3
 * @tipoR es la recolocación utilizada (1=lineal, usa el parámetro @a, 2=cuadrática)
 * @a es el parámetro a de la recolocación lineal
 * @nPasosExtraE son los pasos que tengo que realizar hasta encontrar el elemento a eliminar
 */
void BorrarHash(TablaHash t, char *claveborrar, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a, int* nPasosExtraI);

#endif	// TABLA_HASH_RECOLOCACION_H

