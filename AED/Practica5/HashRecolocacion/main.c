#include <stdio.h>
#include <stdlib.h>
#include "tabla_hash_recolocacion.h"

/// MODIFICACIONES EN main.c
/// Inserción (función insercionArchivo): Variables nColisionesI, nPasosExtraI
/// Búsqueda (función busquedaArchivo): Variables nPasosExtraB

void insercionArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a);
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a);

int main(int argc, char** argv) {
    //variable que almacenan el tipo de función hash (1-2-3)
    unsigned int tipoFH; 
    unsigned int K=256; //factor de ponderación para hash por suma ponderada
    //variable que almacena el tipo de recolocación: lineal (1), o cuadrática (2)
    unsigned int tipoR;

    TablaHash t; //tabla hash

    //////////////////////////////////////////////////////////////////////////////////////
    //Definir las variables nColisionesI, nPasosExtraI y nPasosExtraB e inicializarlas a 0
    //////////////////////////////////////////////////////////////////////////////////////

    //Abrimos el fichero de datos de ejemplo, con 10000 jugadores
    FILE *fp = fopen("jugadores_jdt.csv", "rt");

    //Informo del tamaño de la tabla actual
    printf("Tamaño de la tabla hash: %d\n\n", N);

    //Pregunto al usuario que función hash quiere utilizar
    printf("\tFunción hash a utilizar:"
            "\n\t\t1: método división (suma ascii módulo N)"
            "\n\t\t2: suma ponderada con K=256"
            "\n\t\t3: suma ponderada con otro valor de K ");
    printf("\n\tTipo función hash: ");scanf("%d", &tipoFH);
    if(tipoFH==3){
        printf("\t\tValor de ponderación K (256): ");scanf("%d",&K);
    }
    printf("\nFUNCIÓN HASH %u", tipoFH);
    if (tipoFH==2 || tipoFH==3)
        printf(" con K=%u\n",K);

    //Pregunto al usuario que estrategia de recolocación quiere utilizar
    //Si es lineal, también pido el valor de a
    printf("\n\n\tEstrategia de recolocación (1: Lineal, 2: Cuadrática): ");
    scanf("%d", &tipoR);
    unsigned int a=0; //inicializo a
    if (tipoR == 1) {
        printf("\t\tValor de a (1 para recolocación simple): ");
        scanf("%u", &a);
        printf("\n\nRECOLOCACIÓN LINEAL con a=%u", a);
    } else
        printf("\n\nRECOLOCACIÓN CUADRÁTICA");

    //Inicializo la tabla hash por encadenamiento
    InicializarTablaHash(t);

    printf("\n\t----INSERCIÓN RECOLOCACIÓN----");
    //llamo a la función de inserción en t a partir del archivo
    //////////////////////////////////////////////////////////////////
    //Añadir los parámetros por referencia nColisionesI y nPasosExtraI
    //////////////////////////////////////////////////////////////////
    insercionArchivo(fp, t, tipoFH, K, tipoR, a);
    //////////////////////////////////////////////////////////////////
    //Imprimir nColisionesI y nPasosExtraI
    //////////////////////////////////////////////////////////////////

    rewind(fp); //rebobino

    printf("\n\n\t----BÚSQUEDA RECOLOCACIÓN----");
    //Llamo a la función de búsqueda en t a partir del archivo 
    //////////////////////////////////////////////////////////////////
    //Añadir el parámetros por referencia nPasosExtraB
    //////////////////////////////////////////////////////////////////
    busquedaArchivo(fp, t, tipoFH, K, tipoR, a);
    //////////////////////////////////////////////////////////////////
    //Imprimir nPasosExtraB
    //////////////////////////////////////////////////////////////////

    fclose(fp); //Cierro el archivo    

    return (EXIT_SUCCESS);
}

//Función para insertar usuarios en tabla hash, usando la función hash "tipo"
//////////////////////////////////////////////////////////////////
//Añadir los parámetros por referencia nColisionesI y nPasosExtraI
//////////////////////////////////////////////////////////////////
void insercionArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a) {
    TIPOELEMENTO jugador;
    if (fp) {
        fscanf(fp, "%[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
        while (!feof(fp)) {
            //////////////////////////////////////////////////////////////////////////////////////////
            //Modificar la función InsertarHash para que devuelve: 1 si colisión, 0 en caso contrario
            //y acumular estos valores en nColisionesI
            //Añadir a InsertarHash parámetro nPasosExtraI por referencia
            /////////////////////////////////////////////////////////////////////////////////////////
            InsertarHash(t, jugador, tipoFH, K, tipoR, a);
            fscanf(fp, "%[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}

//Función para buscar un elemento leído del archivo en la tabla t, usando la función hash "tipo"
//////////////////////////////////////////////////////////////////
//Añadir el parámetro por referencia nPasosExtraB
//////////////////////////////////////////////////////////////////
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, unsigned int tipoR, unsigned int a) {
    TIPOELEMENTO jugador;
    if (fp) {
        fscanf(fp, "%[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
        while (!feof(fp)) {
            //El número de colisiones es el mismo que en inserción, hay pasos adicionales al buscar en la lista
            ///////////////////////////////////////////////////////////////////////////////
            //Modificar la función BuscarHash() para que reciba nPasosExtraB por referencia 
            ///////////////////////////////////////////////////////////////////////////////
            BuscarHash(t, jugador.alias, &jugador, tipoFH, K, tipoR, a);
            fscanf(fp, "%[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}