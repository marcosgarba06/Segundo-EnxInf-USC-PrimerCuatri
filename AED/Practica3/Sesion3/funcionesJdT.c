#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionesJdT.h"
#include "abb.h"
#include "lista.h"
#define NAME_LENGTH 50


//struct para el vector de structs de la familia numerosa
typedef struct {
    TLISTA parents; //lista
    TLISTA siblings; //lista
} FAMILIANUMEROSA;

void hacerLista(TLISTA *lista, char *name){
    
    TIPOELEMENTOLISTA elemento;
    
    //printf("¿Desea añadir algun elemento a la lista %s? (s/n): ", name);
    //scanf(" %c", &opcion);
    

    while(1){
            
        printf("%s (o 'fin' para terminar): ", name);
        scanf(" %[^\n\r]", elemento.name);
        
        if (strcmp(elemento.name, "fin") == 0) {
            break; // Salir del bucle si el usuario ingresa "fin"
        }

        insertarElementoLista(lista,finLista(*lista), elemento);
        printf("%s '%s' añadido correctamente\n", name, elemento.name);
    }     
    
    printf("\n");
}


void anadirPersonaje(TABB *arbol) {
    //definicion variables
    TIPOELEMENTOABB nodo;

    
    printf("Nombre del personaje : ");
    scanf(" %[^\n\r]", nodo.name); 

    if(esMiembroAbb(*arbol,nodo)){
        printf("El personaje %s ya existe en la base de datos, no se puede anadir.\n", nodo.name);
        printf("\n");
        return;
    }

    //crear listas
    crearLista(&nodo.parents);
    crearLista(&nodo.siblings);
    crearLista(&nodo.killed);
    crearLista(&nodo.marriedEngaged);

    printf("Tipo de personaje (- en caso de desconocer): ");
    scanf(" %[^\n\r]", nodo.character_type);
    printf("\n");
    
    printf("Casa del personaje (- en caso de desconocer): ");
    scanf(" %[^\n\r]", nodo.house);
    printf("\n");
        

    printf("Es familia real (1 para si, 0 para no): ");
    scanf(" %hu", &nodo.real); //hu lee unsigned short
    printf("\n");

    

    //procesar las listas
    hacerLista(&nodo.parents, "Padres");
    hacerLista(&nodo.siblings, "Hermanos");
    hacerLista(&nodo.killed, "Personas que ha asesinado");
    hacerLista(&nodo.marriedEngaged, "Personas que ha sido pareja");

    printf("Descripcion del personaje (- en caso de desconocer): ");
    scanf(" %[^\n\r]", nodo.description);
    printf("\n");

    insertarElementoAbb(arbol, nodo);

    printf("Personaje '%s' anadido correctamente al arbol.\n", nodo.name);
        
}
    



void _imprimirLista(TLISTA lista){
    TPOSICION posActual, posFinal;
    TIPOELEMENTOLISTA elemento;
    int primero = 1;  // Variable flag para el primer elemento y no dejar una "," al final

    posFinal = finLista(lista);

    for(posActual = primeroLista(lista); posActual != posFinal; posActual = siguienteLista(lista, posActual)){
        recuperarElementoLista(lista, posActual, &elemento);
        if (!primero) {
            printf(",");
        }
        printf(" %s", elemento.name);
        primero = 0;
    }
}

void _imprimirPersonaje(TIPOELEMENTOABB personaje){
    printf("Nombre: %s\n", personaje.name);
    printf("Tipo: %s\n", personaje.character_type);
    printf("Casa: %s\n", personaje.house);
    printf("Familia real: %s\n", personaje.real ? "Si" : "No");

    //Lista de padres

    if (!esListaVacia(personaje.parents)) {
        printf("Padres:");
        _imprimirLista(personaje.parents);
        printf("\n");
    }
    
    /* Imprimir listas en cada caso comprobar que la lista no sea vacia, si lo es se imprime "-" 
    ya que es lo que indica que es un campo desconocido, si tienen elementos se imprime la lista
    con la funcion _imprimirLista creada anteiormente*/
    
    //Lista de hermanos

    if (! esListaVacia(personaje.siblings)) {
        printf("Hermanos:");
        _imprimirLista(personaje.siblings);
        printf("\n");
    }

    //Lista de personas que ha asesinado
    
    if (!esListaVacia(personaje.killed)) {
        printf("Personas que ha asesinado:");
        _imprimirLista(personaje.killed);
        printf("\n");
    }

    //Lista de personas que ha sido pareja
    
    if (esListaVacia(personaje.marriedEngaged)) {
        printf("Personas que ha sido pareja:");
        _imprimirLista(personaje.marriedEngaged);
        printf("\n");
    }

    printf("Descripcion: %s\n", personaje.description);
}


void listadoPersonajes(TABB arbol){
    TIPOELEMENTOABB elemen;
    if (esAbbVacio(arbol)) { //Comprobar que no es vacio el arbol
        //printf("El arbol esta vacio, no hay personajes que mostrar.\n");
        return;
    }else{
        //Usando recorrido InOrden (IRD)
        listadoPersonajes(izqAbb(arbol)); //Primero subarbol izquierdo recirsivamente
        
        leerElementoAbb(arbol, &elemen); //Elemento actual
        _imprimirPersonaje(elemen); 
        printf("\n");

        listadoPersonajes(derAbb(arbol)); //luego subarbol derecho
    }
}

//Funcion que elimina a un personaje de la base de datos
void eliminarPersonaje(TABB *arbol){
    TIPOELEMENTOABB e;
    printf("Introduce el nombre del personaje a eliminar:");
    scanf(" %[^\n]", e.name);

    if(esMiembroAbb(*arbol,e)){
        buscarNodoAbb(*arbol,e.name,&e);

        suprimirElementoAbb(arbol, e);
        printf("El personaje %s, ha sido eliminado con éxito\n", e.name);
    }
    else{
        printf("El personaje que deseas eliminar no ha sido registrado en la base de datos.\n");
    }
}

//Funcion que elimina el retorno de carro al leer una linea de un archivo
void _strip_line ( char * linea ) {
    linea [ strcspn ( linea , "\r\n") ] = 0;
}

//Funcion que procesa una cadena que contiene elementos separados por comas y los inserta en una lista.
void _stripByComma(TLISTA *lista, char *cadena){

    if (cadena == NULL ||strcmp(cadena, "-") == 0) {
        return; 
    } else { 
        TIPOELEMENTOLISTA e;
        char *token = strtok(cadena, ","); 
        while (token != NULL) {
            strncpy(e.name, token, NAME_LENGTH); 
            insertarElementoLista(lista, finLista(*lista), e); 
            token = strtok(NULL, ","); // Obtener el siguiente token
        }
    }
}

//Funcion que carga en el ABB los personajes del archivo
void cargar_archivo(TABB *arbol, int argc, char **argv){
    if (argc != 3 || strcmp(argv[1], "-f") != 0) {
        printf("El programa continuará con una base de datos vacía debido a que no se ha introducido archivo.\n\n");
        return;
    }

    FILE *f = fopen(argv[2], "r"); //Abrir archivo en modo lectura
    if (f == NULL) {
        printf("No se ha podido abrir el archivo %s. El programa continuará con una base de datos vacía.\n\n", argv[2]);
        return;
    }
    
    char line[1024]; //Variable para leer la linea del archivo

    while (fgets(line, sizeof(line), f))
    {    
        TIPOELEMENTOABB personaje;
        _strip_line(line); //Eliminar retorno de carro

        char *name, *character_type, *house, *real, *description;
        char *parents_list, *siblings_list, *killed_list, *marriedEngaged_list;

        name = strtok(line, "|"); //Dividir la linea por el caracter '|'
        character_type = strtok(NULL, "|");
        house = strtok(NULL, "|");
        real = strtok(NULL, "|");
        parents_list = strtok(NULL, "|");
        siblings_list = strtok(NULL, "|");
        killed_list = strtok(NULL, "|");
        marriedEngaged_list = strtok(NULL, "|");
        description = strtok(NULL, "|");

        //Asignar valores a los campos del personaje
        if (name) 
            strncpy(personaje.name, name, NAME_LENGTH);
        if (character_type) 
            strncpy(personaje.character_type, character_type, NAME_LENGTH);
        if (house) 
            strncpy(personaje.house, house, NAME_LENGTH);
        if (real) 
            personaje.real = (unsigned short)atoi(real);

        //Crear listas
        crearLista(&personaje.parents);
        crearLista(&personaje.siblings);
        crearLista(&personaje.killed);
        crearLista(&personaje.marriedEngaged);

        //procesar las listas
        _stripByComma(&personaje.parents, parents_list);
        _stripByComma(&personaje.siblings, siblings_list);
        _stripByComma(&personaje.killed, killed_list);
        _stripByComma(&personaje.marriedEngaged, marriedEngaged_list);

        if (description) 
            strncpy(personaje.description, description, 3*NAME_LENGTH);
        
        insertarElementoAbb(arbol, personaje); //Insertar personaje en el ABB

    }
    fclose(f); //Cerrar archivo
    printf("Base de datos cargada correctamente desde el archivo %s\n\n", argv[2]);
}

//Funcion que imprime la lista en el archivo txt
void imprimirListaArch(TLISTA list, FILE *f){
   
    TIPOELEMENTOLISTA elemento;
    TPOSICION posActual, posFinal;

    posActual = primeroLista(list);
    posFinal = finLista(list);

    while (posFinal != posActual)
    {
        recuperarElementoLista(list, posActual, &elemento);
        posActual = siguienteLista(list, posActual);
        if (posActual == posFinal){
            fprintf(f, "%s|", elemento.name); //Si es el ultimo elemento de la lista poner  /
        }
        else{
            fprintf(f, "%s,", elemento.name); //Si no es el ultimo elemento de la lista poner ,
        }
    }
} 

//REVISAR PARA QUE NO IMPRIMA UN SALTO DE LINEA SI ES EL ULTIMO PERSONAJE
void imprimirPersonajeArch(TIPOELEMENTOABB personaje, FILE *f, int *primero){
    if (!(*primero)) //Si no es el primer personaje, poner un salto de linea antes
        fprintf(f, "\n");
    *primero = 0;

    fprintf(f, "%s|%s|%s|%hu|", personaje.name, personaje.character_type, personaje.house, personaje.real);
    
    //Imprimir listas, si son vacias imprimir "-|", si no imprimir la lista con la funcion imprimirListaArch
    
        *primero = 0; //Si es el primer personaje, cambiar el valor de primero a 0 para que los siguientes personajes no lo sean
    if (!esListaVacia(personaje.parents)) //lista padres
        imprimirListaArch(personaje.parents, f);
    else
        fprintf(f, "-|");

    if (!esListaVacia(personaje.siblings)) //lista hermanos
         imprimirListaArch(personaje.siblings, f);
    else
        fprintf(f, "-|");
       
    if (!esListaVacia(personaje.killed)) //lista personas que ha asesinado
        imprimirListaArch(personaje.killed, f);
    else
        fprintf(f, "-|");

    if (!esListaVacia(personaje.marriedEngaged)) //lista personas que ha sido pareja
        imprimirListaArch(personaje.marriedEngaged, f);   
    else
        fprintf(f, "-|");

    fprintf(f, "%s", personaje.description); //Descripcion al final y salto de linea
}

//Imprime en el archivo todos los personajes del arbol
void imprimirArchivoAux(TABB arbol, FILE *f, int *primero){
    if (esAbbVacio(arbol))
        return;

    TIPOELEMENTOABB elemen;

    imprimirArchivoAux(izqAbb(arbol), f, primero);
    imprimirArchivoAux(derAbb(arbol), f, primero);
    leerElementoAbb(arbol, &elemen);
    imprimirPersonajeArch(elemen, f, primero);
}

//Funcion que imprime en el archivo los personajes de la base de datos del ABB
//Se hacen funciones separadas para que no aparezca 3 veces el scanf
void imprimirArchivo(TABB arbol,  int argc, char **argv){
    if (argc != 3 || strcmp(argv[1], "-f") != 0) {
        char filename[100];
        printf("No se ha indicado archivo por linea de comandos\n");
        printf("Nombre del archivo para guardar la base de datos:");
        scanf(" %s", filename);
        argv[1] = "-f";
        argv[2] = filename;
    }
    FILE *f = fopen(argv[2], "w"); //Abrir archivo en modo escritura
    int primero = 1;
    imprimirArchivoAux(arbol, f, &primero);
    fclose(f);
}

//Funcion que busca un personaje por nombre 
void buscarPersonaje(TABB arbol){
    TIPOELEMENTOABB e, temp;
    printf("Introduce el nombre del personaje a buscar:");
    scanf(" %[^\n\r]", temp.name);

    if(esMiembroAbb(arbol,temp)){
        buscarNodoAbb(arbol,temp.name,&e);
        _imprimirPersonaje(e);
    }
    else{
        printf("El personaje no ha sido registrado en la base de datos.\n");
    }
    printf("\n");
}

//Funcion auxiliar para listar los personajes segun su tipo
void _listaTipoAux(TABB arbol, char *tipo){
    TIPOELEMENTOABB elemen;
    if (esAbbVacio(arbol)) {
        return;
    }else{
        //InOrden (IRD)
        _listaTipoAux(izqAbb(arbol), tipo); //izquierda
        
        leerElementoAbb(arbol, &elemen); //raiz
        if (strcmp(elemen.character_type, tipo) == 0){
            _imprimirPersonaje(elemen); 
            printf("\n");
        }

        _listaTipoAux(derAbb(arbol), tipo); //derecha
    }
}
//Funcion que lista los personajes segun su tipo
void listadoTipo(TABB arbol){
    TIPOELEMENTOABB elemen;
    char opcion;
    
    printf("Introduce el tipo de personaje a listar (1: persona , 2: gigante , 3: lobo , 4: dragon , 5: criatura, -: desconocido):");
    scanf(" %c", &opcion);
    printf("\n");

    switch (opcion){
        case '1': strncpy(elemen.character_type, "persona", NAME_LENGTH); break;
        case '2': strncpy(elemen.character_type, "gigante", NAME_LENGTH); break;
        case '3': strncpy(elemen.character_type, "lobo", NAME_LENGTH); break;
        case '4': strncpy(elemen.character_type, "dragon", NAME_LENGTH); break;
        case '5': strncpy(elemen.character_type, "criatura", NAME_LENGTH); break;
        case '-': strncpy(elemen.character_type, "-", NAME_LENGTH); break;
        default: break;
    }   
    
    _listaTipoAux(arbol, elemen.character_type);
    printf("\n");    
}

void copiarLista(TLISTA original, TLISTA *copia){
    crearLista(copia);
    TIPOELEMENTOLISTA elemento;
    TPOSICION posActual, posFinal;
    posActual = primeroLista(original);
    posFinal = finLista(original);
    while (posActual != posFinal)
    {
        recuperarElementoLista(original, posActual, &elemento);
        insertarElementoLista(copia, finLista(*copia), elemento);
        posActual = siguienteLista(original, posActual);
    }

}
void modificarPersonaje(TABB *arbol){
    TIPOELEMENTOABB e, temp;
    TIPOELEMENTOLISTA elemento;
    printf("Introduce el nombre del personaje a modificar:");
    scanf(" %[^\n\r]", temp.name);
    char opcion;

    if(!esMiembroAbb(*arbol,temp)){
        printf("El personaje que deseas modificar no ha sido registrado en la base de datos.\n");
        return;
    }

    buscarNodoAbb(*arbol,temp.name,&e);
    _imprimirPersonaje(e);

    printf("¿Qué quieres modificar (n/c/h/r/p/s/k/m/d)? ");
    scanf(" %c", &opcion);

    switch (opcion)
    {
    case 'n':
        TIPOELEMENTOABB new;
        new = e; //Copiar el personaje actual en uno nuevo
        printf("Nuevo nombre del personaje: ");
        scanf(" %[^\n\r]", new.name);

        //copiar listas
        copiarLista(e.parents, &new.parents);
        copiarLista(e.siblings, &new.siblings);
        copiarLista(e.killed, &new.killed);
        copiarLista(e.marriedEngaged, &new.marriedEngaged);
        
        //Modificar el personaje en el ABB
        suprimirElementoAbb(arbol, e); //Eliminar el personaje antiguo
        insertarElementoAbb(arbol, new); //Insertar el personaje con el nuevo nombre        
        break;
    case 'c':
        printf("Nuevo tipo de personaje (- en caso de desconocer): ");
        scanf(" %[^\n\r]", e.character_type);
        modificarElementoAbb(*arbol, e);
        break;
    case 'h':
        printf("Nueva casa del personaje (- en caso de desconocer): ");
        scanf(" %[^\n\r]", e.house);
        modificarElementoAbb(*arbol, e);
        break;
    case 'r':
        printf("Es familia real (1 para si, 0 para no): ");
        scanf(" %hu", &e.real); //hu lee unsigned short
        modificarElementoAbb(*arbol, e);
        break;
    case 'p':
        printf("Nuevo elemento para la lista de padres: ");
        scanf(" %[^\n\r]", elemento.name);
        modificarElementoAbb(*arbol, e);
        insertarElementoLista(&e.parents,finLista(e.parents), elemento);

        break;
    case 's':
        printf("Nuevo elemento para la lista de hermanos: ");
        scanf(" %[^\n\r]", elemento.name);
        insertarElementoLista(&e.siblings,finLista(e.siblings), elemento);
        modificarElementoAbb(*arbol, e);
        break;
    case 'k':
        printf("Nuevo elemento para la lista de personas que ha asesinado: ");
        scanf(" %[^\n\r]", elemento.name);
        insertarElementoLista(&e.killed,finLista(e.killed), elemento);
        break;
    case 'm':
        printf("Nuevo elemento para la lista de personas que ha sido pareja: ");
        scanf(" %[^\n\r]", elemento.name);
        insertarElementoLista(&e.marriedEngaged,finLista(e.marriedEngaged), elemento);
        modificarElementoAbb(*arbol, e);
        break;
    case 'd':
        printf("Nueva descripcion del personaje (- en caso de desconocer): ");
        scanf(" %[^\n\r]", e.description);
        modificarElementoAbb(*arbol, e);
        break;
    default:
        break;
    }
}

int _tamanhoFamilia(TIPOELEMENTOABB e){
    return (longitudLista(e.parents) + longitudLista(e.siblings) + 1);
}

//Funcion que busca la mayor familia (suma de padres y hermanos) en el arbol
void _buscarMayorFamilia(TABB arbol, int *max){
    if (esAbbVacio(arbol)) {
        return;
    }else{
        TIPOELEMENTOABB elemen;
        int num;

        //Recorrido preorden (RID)
        leerElementoAbb(arbol, &elemen); //raiz
        num = _tamanhoFamilia(elemen);
        if (num > *max)
            *max = num;
        
        _buscarMayorFamilia(izqAbb(arbol), max); //izquierda
        _buscarMayorFamilia(derAbb(arbol), max); //derecha
    }
}


int _listasIguales(TLISTA l1, TLISTA l2){
    if (longitudLista(l1) != longitudLista(l2))
        return 0;
    
    TPOSICION pos1, pos2;
    TIPOELEMENTOLISTA elemento, elemento2;
    int encontrado = 0;

    pos1 = primeroLista(l1);

    //Comparar las listas elemento a elemento, cada elemento de la lista 1
    //se debe comparar con cada elemento de la lista 2 porque las listas pueden
    //estar en distinto orden  pero tener los mismos elementos
    while (pos1 != finLista(l1)){
        recuperarElementoLista(l1, pos1, &elemento);
        pos2 = primeroLista(l2);
        while (pos2 != finLista(l2))
        {
            recuperarElementoLista(l2, pos2, &elemento2);
            if (strcmp(elemento.name, elemento2.name) == 0){
                encontrado = 1;
                break;
            }
            pos2 = siguienteLista(l2, pos2);
        }
        if (!encontrado)
            return 0;
        pos1 = siguienteLista(l1, pos1);
        
    }
    //Si se han encontrado todos los elementos de la lista 1 en la lista 2, son iguales
    return 1;
}

//Comprobar si las listas de padres y hermanos son iguales
//Si alguna de las dos listas no es igual, no es la misma familia
int _esFamiliaRepetida(FAMILIANUMEROSA fam, TLISTA padres, TLISTA hermanos){
   
    if (!_listasIguales(fam.parents, padres)) return 0;
    else if (!_listasIguales(fam.siblings, hermanos)) return 0;
    else return 1;
}

void _familiaNumerosaAVectorStruct(TABB arbol, int max, FAMILIANUMEROSA *familias, int *numFamilas, char nombres[][NAME_LENGTH]){
    if(esAbbVacio(arbol)) {
        return;
    }else{
        TIPOELEMENTOABB elemen;
        int tamFam, repe, i;
        //Recorrido preorden (RID)
        leerElementoAbb(arbol, &elemen); //raiz
        tamFam = _tamanhoFamilia(elemen);
        
        //Solo se comparan las familias que tengan el maximo numero de miembros
        if (tamFam == max){
            repe = 0;
            for (i = 0; i < *numFamilas; i++){
                //comprobar si la familia ya esta en el vector de structs
                if (_esFamiliaRepetida(familias[i], elemen.parents, elemen.siblings)){
                    repe = 1;
                    break;
                }
            }
            //Si no esta repetida, se añade al vector de structs
            if (!repe){
                FAMILIANUMEROSA fam;
                copiarLista(elemen.parents, &fam.parents);
                copiarLista(elemen.siblings, &fam.siblings);
                familias[*numFamilas] = fam;
                strncpy(nombres[*numFamilas], elemen.name, NAME_LENGTH); //Copiar nombre al array
                (*numFamilas)++;
            }
        }
    }
    _familiaNumerosaAVectorStruct(izqAbb(arbol), max, familias, numFamilas, nombres); //izquierda
    _familiaNumerosaAVectorStruct(derAbb(arbol), max, familias, numFamilas, nombres); //derecha
}

//FALTA COMPROBAR QUE SON DE TIPO PERSONA
void familiaNumerosa(TABB arbol){
    int max = 0;
    _buscarMayorFamilia(arbol, &max);
    FAMILIANUMEROSA familias[30]; //Vector de structs para almacenar las familias numerosas
    int numFamilas = 0;
    char nombres[30][NAME_LENGTH]; //Array para almacenar los nombres de los personajes con la familia numerosa
    _familiaNumerosaAVectorStruct(arbol, max, familias, &numFamilas, nombres);

    //Imprimir las familias numerosas
    if (numFamilas == 0){
        printf("No hay familias en la base de datos.\n");
        return;
    }else if (numFamilas == 1)
    {
        printf("La familia mas numerosa tiene %d miembros:\n", max);
    }else{
        printf("Las familias mas numerosas tienen %d miembros y son:\n", max);
       
    }
    for  (int i = 0; i < numFamilas; i++){
        printf("Familia %d:\n", i+1);
        printf("Padres:");
        _imprimirLista(familias[i].parents);
        printf("\nHermanos:");
        //Hacer que imprima el personaje con el que se ha encontrado la lista al final
        TPOSICION pos = primeroLista(familias[i].siblings);
        TIPOELEMENTOLISTA elemento;
        int primero = 1;  // Variable flag para el primer elemento
        while (pos != finLista(familias[i].siblings)){
            recuperarElementoLista(familias[i].siblings, pos, &elemento);
            if (!primero) printf(", ");
        
            printf("%s", elemento.name);
            primero = 0;
            pos = siguienteLista(familias[i].siblings, pos);
        }
        //Imprimir el personaje con el que se ha encontrado la lista
        if (!primero) printf(", "); //Si ya se ha impreso algun hermano, poner una coma
        printf("%s\n", nombres[i]);
        printf("\n");
    }
    //Liberar memoria de las listas dentro del vector de structs
    for (int i = 0; i < numFamilas; i++){
        destruirLista(&familias[i].parents);
        destruirLista(&familias[i].siblings);
    }
}