#include <stdlib.h>

//Implementacion TAD pila

//CONTENIDO DE CADA ELEMENTO DE LA PILA
//MODIFICAR: PARA LA PILA DE OPERADORES: char
//MODIFICAR: PARA LA PILA DE OPERANDOS: abin
typedef int tipoelemPila;
///////////////////////////////////////////////////////

//Definicion del tipo de datos pila
struct tipo_celda {
    tipoelemPila elemento;
    struct tipo_celda * sig;
};

typedef struct tipo_celda * pila;
/////////////////////////////////////////////////////////

void crearPila(pila *P) {
    *P = NULL;
}

void destruirPila(pila *P) {
    pila aux;
    aux = *P;
    while (aux != NULL) {
        aux = aux->sig;
        free(*P);
        *P = aux;
    }
}

unsigned esVaciaPila(pila P) {
    return P == NULL;
}

tipoelemPila tope(pila P) {
    if (!esVaciaPila(P)) /*si pila no vacia*/
        return P->elemento;
}

void push(pila *P, tipoelemPila E) {
    pila aux;
    aux = (pila) malloc(sizeof (struct tipo_celda));
    aux->elemento = E;
    aux->sig = *P;
    *P = aux;
}

void pop(pila *P) {
    pila aux;
    if (!esVaciaPila(*P)) /*si pila no vacia*/ {
        aux = *P;
        *P = (*P)->sig;
        free(aux);
    }
}
