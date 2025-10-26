#include <stdlib.h>

//Implementacion TAD pila

//CONTENIDO DE CADA ELEMENTO DE LA PILA
//MODIFICAR: PARA LA PILA DE OPERADORES: char
//MODIFICAR: PARA LA PILA DE OPERANDOS: abin
typedef char tipoelemPilaOperadores;
///////////////////////////////////////////////////////

//Definicion del tipo de datos pila
struct tipo_celda {
    tipoelemPilaOperadores elemento;
    struct tipo_celda * sig;
};

typedef struct tipo_celda * pilaOperadores;
/////////////////////////////////////////////////////////

void crearPilaOperadores(pilaOperadores *P) {
    *P = NULL;
}

void destruirPilaOperadores(pilaOperadores *P) {
    pilaOperadores aux;
    aux = *P;
    while (aux != NULL) {
        aux = aux->sig;
        free(*P);
        *P = aux;
    }
}

unsigned esVaciaPilaOperadores(pilaOperadores P) {
    return P == NULL;
}

tipoelemPilaOperadores topeOperadores(pilaOperadores P) {
    if (!esVaciaPilaOperadores(P)) /*si pila no vacia*/
        return P->elemento;
}

void pushOperadores(pilaOperadores *P, tipoelemPilaOperadores E) {
    pilaOperadores aux;
    aux = (pilaOperadores) malloc(sizeof (struct tipo_celda));
    aux->elemento = E;
    aux->sig = *P;
    *P = aux;
}

void popOperadores(pilaOperadores *P) {
    pilaOperadores aux;
    if (!esVaciaPilaOperadores(*P)) /*si pila no vacia*/ {
        aux = *P;
        *P = (*P)->sig;
        free(aux);
    }
}