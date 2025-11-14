// Ejercicio 5
// Código de Marcos García Blanco e Inés Landeira Rodríguez

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Para sin
#include <unistd.h> // Para getpid

/*
* Para compilar el programa:
*       · Forma Dinámica: gcc -o 5 5.c -lm 
*       · Forma Estática: gcc -static -o 5 5.c -lm
*/
int main(int argc, char const *argv[])
{

    // Mostrar mensaje para ver el mapa de memoria
    printf("Puedes comprobar el mapa de memoria usando cat /proc/%d/maps\n", getpid());

    double sen = 0.0, valor = 0.0;

    printf("Elige un valor para calcular su seno (en radianes): ");
    scanf("%lf", &valor);
    
    // Calcular el seno 
    sen = sin(valor);
    printf("sin(%lf) = %lf\n", valor, sen);

    // Esperar para finalizar para poder ver el mapa de memoria
    printf("Pulsa enter para finalizar...");
    scanf("%*c"); // Ignorar el caracter de nueva línea
    return 0;
}
