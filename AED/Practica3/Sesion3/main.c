//En TIPOELEMENTOABB incluye ya los campos de nombrenn no  hace falta crear variables nuevas para almacenar el nombre
//scanf("%[^\n\r]", personaje.name) 
//NO hay que definir una nueva vaiable y despues hacer un strcpy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "funcionesJdT.h"

int main(int argc, char** argv)
{
    char opcion;
    TABB arbol;
    crearAbb(&arbol);
    cargar_archivo(&arbol, argc, argv);
    printf("Bienvenid@ al programa de gestion de personajes Juego de Tronos.\n");
    do
    {
        printf("----------------------------------\n");
        printf("Elija una de las opciones:\n");
        printf("A/a. Anadir personaje\n");
        printf("L/l. Listar personajes\n");
        printf("B/b. Buscar personaje por nombre\n");
        printf("T/t Listar personaje por tipo\n");
        printf("M/m. Modificar personaje\n");
        printf("F/f. Buscar la familia mas numerosa\n");
        printf("E/e. Eliminar personaje\n");
        printf("S/s. Salir del programa de JdT\n");
        printf("\n--------------------------------\n");
        printf("\nOpcion: ");
        scanf(" %c", &opcion);
        printf("\n");

        switch (opcion)
        {
        case 'A': case 'a':
            anadirPersonaje(&arbol);
            break;
            
        case 'L': case 'l':
            listadoPersonajes(arbol);
            break;

        case 'B': case 'b':
            buscarPersonaje(arbol);
            break;

        case 'T': case 't':
            listadoTipo(arbol);
            break;
            
        case 'M': case 'm':
            modificarPersonaje(&arbol);
            break;
        case 'F': case 'f':
            familiaNumerosa(arbol);
            break;
        case 'E': case 'e':
            eliminarPersonaje(&arbol);
            break;

        case 'S': case 's':
            imprimirArchivo(arbol, argc, argv);
            destruirAbb(&arbol);
            printf("Saliendo del programa de JdT...\n");
            break;

        default:
            printf("Opcion no valida, intente de nuevo.\n");
            break;
        }

    } while (opcion!='S' && opcion!='s');
    return 0;
}
