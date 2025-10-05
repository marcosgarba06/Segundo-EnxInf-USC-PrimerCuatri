#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abin.h"
#include "pilaOperandos.h"
#include "pilaOperadores.h"

//Devuelve la prioridad del operador dentro de la pila.
//Prioridad('(')=0
int prioridadDentro(char op) {
    int prior;
    switch (op) {
        case '^': prior = 3;
            break;
        case '*': case '/': prior = 2;
            break;
        case '+': case '-': prior = 1;
            break;
        case '(': prior = 0; //nunca va a entrar en la pila, provoca vaciado
            break;
    }
    return prior;
}

//Devuelve la prioridad del operador fuera de la pila.
//Prioridad('(')=4
int prioridadFuera(char op) {
    int prior;
    switch (op) {
        case '^': prior = 3;
            break;
        case '*': case '/': prior = 2;
            break;
        case '+': case '-': prior = 1;
            break;
        case '(': prior = 4; //para que SIEMPRE entre en la pila
            break;
    }
    return prior;
}

//Devuelve 1 si c es un operador: +, -, /, *, ^, ()
unsigned esOperador(char c) {
    unsigned ope;
    switch (c) {
        case '^': case '*': case '/': case '+': case '-': case '(':
            ope = 1;
            break;
        default:
            ope = 0;
    }
    return ope;
}

//Devuelve 1 si c es un operando: mayúsculas y minúsculas
//completar para caracteres 0 a 9
unsigned esOperando(char c) {
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (  c>= 48 && c <= 57 ) ) //Añadir numeros del 0 al 9
        return 1;
    else
        return 0;
}

//Crea y devuelve un arbol auxiliar de opeandos y desapila pila operadores 
//Se hace otra funcion para mejorar modularidad ya que se usa varias veces
abin crearArbolAux(pilaOperadores *pilaOperadores, pilaOperandos *pilaOperandos) {
    abin caux;
    crear(&caux);//crear arbol auxiliar

    char op = topeOperadores(*pilaOperadores); //se guarda el tope de la pila

    insizq(&caux, op); //se introduce en el arbol (da igual si der o izq porque es raiz)
    popOperadores(pilaOperadores); //se desapila

    //inserto a la derecha (con la nueva función insArbolder() el tope de pilaOperandos, desapilándolo.
    if (!esVaciaPilaOperandos(*pilaOperandos)) {
        insArbolder(&caux, topeOperandos(*pilaOperandos)); //se inserta el operando derecho
        popOperandos(pilaOperandos); //se desapila
    }

    //Inserto a la izquierda (con la nueva función insArbolizq() el tope de pilaOperandos, desapilándolo.
    if (!esVaciaPilaOperandos(*pilaOperandos)) {
        insArbolizq(&caux, topeOperandos(*pilaOperandos)); //se inserta el operando izquierdo
        popOperandos(pilaOperandos); //se desapila
    }

    pushOperandos(pilaOperandos, caux); //inserto el subarbol en la pila de operandos
    return caux; //devuelvo el arbol
}

//Recibe una cadena de caracteres y devuelve el arbol de expresion
abin arbolExpresion(char *expr_infija) {

    //Crear pilas de operadores (char) y operandos (abin)
    pilaOperandos pilaOperadores;
    pilaOperandos pilaOperandos;
    crearPilaOperadores(&pilaOperadores);
    crearPilaOperandos(&pilaOperandos);
    
    //Crear arbol que contiene la expresion
    abin arbolExpresion;
    abin aux;
    crear(&arbolExpresion);

    //crear un puntero a la cadena para recorrerla
    char *p;
    p = expr_infija;

    //Recorrer la cadena hasta el final
    while (*p != '\0') {
        char c = *p; //c = contenido al que apuunta el puntero p
        if (esOperando(c)) {
            //Si es opeando se inserta en la pila de operandos y
            crear(&aux);
            insizq(&aux, c);
            pushOperandos(&pilaOperandos, aux);

        }else if (esOperador(c)){
            while (!esVaciaPilaOperadores(pilaOperadores) && prioridadFuera(c) <= prioridadDentro(topeOperadores(pilaOperadores))) {
                //crear el arbol aux introduciendo caux como informacion
                aux = crearArbolAux(&pilaOperadores, &pilaOperandos);
                
                //comprobar si se ha creado el arbol aux de manera correcta
                if(aux == NULL) return NULL;
                
            }

            //apilar el operador c en pilaOperadores
            pushOperadores(&pilaOperadores, c);

        }else if(c == ')'){
            //mientras el tope de la pilaOperadores no sea '(' y en casi de que sea '(' desapilar y guardar el tope en aux
            while (!esVaciaPilaOperadores(pilaOperadores) && topeOperadores(pilaOperadores) != '('){
                //guadar el tope en aux
                aux = crearArbolAux(&pilaOperadores, &pilaOperandos);
                
                if (aux == NULL) return NULL; //comprobar si se ha creado el arbol aux de manera correcta
                
                //desapilar si no es vacia (para que no haya error)
                if (!esVaciaPilaOperadores(pilaOperadores)) popOperadores(&pilaOperadores);

            }
            //desapilar el '(' de la pilaOperadores
            if (!esVaciaPilaOperadores(pilaOperadores)) popOperadores(&pilaOperadores);
            }
            p++; //avanzar el puntero al siguiente caracter
        }
        //vaciar pila operadores con el while
        while(!esVaciaPilaOperadores(pilaOperadores)){
            //guardar el tope en aux y desapilar
            aux = crearArbolAux(&pilaOperadores, &pilaOperandos);
            if (aux == NULL) return NULL; //comprobar si se ha creado el arbol aux de manera correcta
        }
        
        destruirPilaOperadores(&pilaOperadores);
        arbolExpresion = topeOperandos(pilaOperandos);
        destruirPilaOperandos(&pilaOperandos);
        return arbolExpresion;
    }



