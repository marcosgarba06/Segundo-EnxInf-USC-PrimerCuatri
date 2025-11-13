#include <stdio.h>
#include <stdlib.h>
#include "tabla_hash_encadenamiento.h"

//Hay que añadir como parámetro la variable nColisionesI: número de colisiones en inserción
void insercionArchivo(FILE *fp, TablaHash *t, unsigned int tipo, unsigned int K, int* nColisionesI);
//Hay que añadir como parámetro la variable nPasosExtraB: número de pasos extra en búsqueda
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipo, unsigned int K, int* nPasosExtraB);

int main(int argc, char** argv) {
    unsigned int tipoFH; //tipo de función hash: 1-2-3
    unsigned int K=256; //factor de ponderación para hash por suma ponderada

    TablaHash t; //tabla hash
    
    //////////////////////////////////////////////////////////////////////
    //Inicializar a 0 las variables nColisionesI y nPasosExtraB
    //////////////////////////////////////////////////////////////////////
    int nColisionesI = 0, nPasosExtraB = 0;

    //Abrimos el fichero de datos de ejemplo, con 10000 jugadores
    FILE *fp = fopen("jugadores_jdt.csv", "rt");

    //Informo del tamaño de la tabla actual
    printf("Tamaño de la tabla hash: %d\n",N);
    
    //Pregunto al usuario que función hash quiere utilizar
    printf("Función hash a utilizar:"
            "\n\t1: método división (suma ascii módulo N)"
            "\n\t2: suma ponderada con K=256"
            "\n\t3: suma ponderada con otro valor de K ");
    printf("\nTipo función hash: ");scanf("%d", &tipoFH);
    if(tipoFH==3){
        printf("\tValor de ponderación K (256): ");scanf("%d",&K);
    }
    printf("\nFUNCIÓN HASH %u", tipoFH);
    if (tipoFH==2 || tipoFH==3)
        printf(" con K=%u\n",K);

    //Inicializo la tabla hash por encadenamiento
    InicializarTablaHash(t);

    printf("\n\t----INSERCIÓN ENCADENAMIENTO----");
    //llamo a la función de inserción en t a partir del archivo y el tipo de función hash a utilizar
    //////////////////////////////////////////////////////
    //Añadir como parámetro por referencia nColisionesI
    //////////////////////////////////////////////////////
    insercionArchivo(fp, &t, tipoFH, K, &nColisionesI);
    //////////////////////////////////////////////////////
    //Imprimir nColisionesI
    //////////////////////////////////////////////////////
    printf("\n\t  Número colisións: %d\n", nColisionesI);

    rewind(fp); //rebobino

    printf("\n\n\t----BÚSQUEDA ENCADENAMIENTO----");
    //Llamo a la función de búsqueda en t a partir del archivo y el tipo de función hash a utilizar
    //////////////////////////////////////////////////////
    //Añadir como parámetro por referencia nPasosExtraB
    //////////////////////////////////////////////////////
    busquedaArchivo(fp, t, tipoFH, K, &nPasosExtraB);
    //////////////////////////////////////////////////////
    //Imprimir nPasosExtraB
    //////////////////////////////////////////////////////
    printf("\n\t  Número pasos extra: %d\n", nPasosExtraB);
    
    DestruirTablaHash(t); //Destruyo la tabla
    fclose(fp); //Cierro el archivo    

    return (EXIT_SUCCESS);
}

//Función para insertar usuarios en tabla hash, usando la función hash "tipo"
///////////////////////////////////////
//Añadir el parámetro por referencia nColisionesI
///////////////////////////////////////
void insercionArchivo(FILE *fp, TablaHash *t, unsigned int tipoFH, unsigned int K, int* nColisionesI) {
    TIPOELEMENTO jugador;
    if (fp) {
        fscanf(fp, " %[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
        while (!feof(fp)) {
            //////////////////////////////////////////////////////////////////////////////////////////
            //Modificar la función InsertarHash para que devuelve: 1 si colisión, 0 en caso contrario
            //y acumular estos valores en nColisionesI
            /////////////////////////////////////////////////////////////////////////////////////////
            *nColisionesI += InsertarHash(t, jugador, tipoFH, K); 
            fscanf(fp, " %[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}

//Función para buscar un elemento leído del archivo en la tabla t, usando la función hash "tipo"
//////////////////////////////////////////////////////////////////
//Añadir el parámetro por referencia nPasosExtraB
//////////////////////////////////////////////////////////////////
void busquedaArchivo(FILE *fp, TablaHash t, unsigned int tipoFH, unsigned int K, int* nPasosExtraB) {
    TIPOELEMENTO jugador;
    int aux = 0;
    if (fp) {
        fscanf(fp, " %[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
        while (!feof(fp)) {
            //////////////////////////////////////////////////////////////////////////////////////////
            //Añadir a BuscarHash() el parámetro por referencia nPasosExtraB
            BuscarHash(t, jugador.alias, &jugador, tipoFH, K, &aux);
            fscanf(fp, " %[^,] , %s , %s", jugador.nombre, jugador.alias, jugador.correo);
            *nPasosExtraB += aux;
        }
    } else {
        printf("El archivo no ha podido abrirse\n");
    }
}