#include <stdlib.h>
#include "lista.h"

struct celda {
    TIPOELEMENTO elemento;
    struct celda *sig;
};

struct l {
    TPOSICION inicio;
    unsigned longitud;
    TPOSICION fin;
};


void crearLista(TLISTA *l) {
    (*l) = (TLISTA) malloc(sizeof (struct l));
    (*l)->inicio = (TPOSICION) malloc(sizeof (struct celda));
    (*l)->fin = (*l)->inicio;
    ((*l)->fin)->sig = NULL;
    (*l)->longitud = 0;
}

void destruirLista(TLISTA *l) {
    (*l)->fin = (*l)->inicio;
    while ((*l)->fin != NULL) {
        (*l)->fin = ((*l)->fin)->sig;
        free((*l)->inicio);
        (*l)->inicio = (*l)->fin;
    }
    free(*l);
    *l = NULL;
}

unsigned existeLista(TLISTA l) {
    if (l != NULL) return 1;
    return 0;
}

unsigned esListaVacia(TLISTA l) {
    if (l->longitud == 0) return 1;
    return 0;
}

TPOSICION primeroLista(TLISTA l) {
    return (l->inicio);
}

TPOSICION siguienteLista(TLISTA l, TPOSICION p) {
    return (p->sig);
}

TPOSICION finLista(TLISTA l) {
    return (l->fin);
}

TPOSICION anteriorLista(TLISTA l, TPOSICION p) {
    TPOSICION q;
    q = l->inicio;
    while (q->sig != p) {
        q = q->sig;
    }
    return q;
}

void recuperarElementoLista(TLISTA l, TPOSICION p, TIPOELEMENTO *e) {
    *e = (p->sig)->elemento;
}

unsigned longitudLista(TLISTA l) {
    return (l->longitud);
}

void insertarElementoLista(TLISTA *l, TPOSICION p, TIPOELEMENTO e) {
    TPOSICION q;
    q = p->sig;
    p->sig = (TPOSICION) malloc(sizeof (struct celda));
    (p->sig)->elemento = e;
    (p->sig)->sig = q;
    if (q == NULL) (*l)->fin = p->sig;
    (*l)->longitud = (*l)->longitud + 1;
}

void suprimirElementoLista(TLISTA *l, TPOSICION p) {
    TPOSICION q;
    q = p->sig;
    p->sig = q->sig;
    if (p->sig == NULL) (*l)->fin = p;
    free(q);
    (*l)->longitud = (*l)->longitud - 1;
}

void modificarElementoLista(TLISTA *l, TPOSICION p, TIPOELEMENTO e) {
    (p->sig)->elemento = e;
}





