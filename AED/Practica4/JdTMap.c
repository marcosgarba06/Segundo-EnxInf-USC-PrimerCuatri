#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

//Opción a del menú, introducir un vertice en el grafo

void introducir_vertice(grafo *G) {
    tipovertice v1;
    
    //Para introducir un vértice pedimos la ciudad y la región en la que está ya que tipovertice es una estructura con esos dos campos
    printf("Introduce nombre de la ciudad: ");
    scanf(" %[^\n]", v1.name);  

    printf("Introduce región: ");
    scanf(" %[^\n]", v1.region);
    
    //Comprobar si el vertice ya esta en el grafo
    if (existe_vertice(*G, v1))
        printf("Ese vertice ya esta en el grafo\n");
    else
        insertar_vertice(G, v1);
}

//Opción b del menú, eliminar un vértice del grafo

void eliminar_vertice(grafo *G) {
    tipovertice v1;
    
    //Para eliminar un vértice pedimos la ciudad y la región en la que está ya que tipovertice es una estructura con esos dos campos
    printf("Introduce nombre de la ciudad: ");
    scanf(" %[^\n]", v1.name);

    printf("Introduce región: ");
    scanf(" %[^\n]", v1.region);

    //Comprobar si el vertice existe en el grafo, si no existe, da un mensaje de error    
    if (existe_vertice(*G, v1))
        borrar_vertice(G, v1);
    else
        printf("Ese vertice no existe en el grafo\n");
}

//Opción c del menú, crear una relación entre dos vértices

void nuevo_arco(grafo *G) {
    tipovertice v1, v2;
    tipoarco arco;

    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1-->vertice2\n");
    
    //Vértice origen del arco
    printf("Introduce nombre de la ciudad origen: ");
    scanf(" %[^\n]", v1.name);
    
    if (!existe_vertice(*G, v1)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v1.name, v1.region);
        return;
    }

    //Vértice destino del arco
    printf("Introduce nombre de la ciudad destino: ");
    scanf(" %[^\n]", v2.name);
    
    /*
    printf("Introduce región destino: ");
    scanf(" %[^\n]", v2.region);*/
    
    if (!existe_vertice(*G, v2)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v2.name, v2.region);
        return;
    }

    //Datos del arco
    printf("Distancia (km): ");
    scanf("%f", &arco.distancia);

    printf("Tipo de conexion ('t' tierra, 'm' mar): ");
    scanf(" %c", &arco.tipoconexion);

    if (arco.tipoconexion != 't' && arco.tipoconexion != 'm' && arco.tipoconexion != 'T' && arco.tipoconexion != 'M') {
        printf("La conexión entre 2 ciudades solo puede ser por mar o por tiera.\n");
        return;
    }
    //Creación del arco
    if (!son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        crear_arco(G, posicion(*G, v1), posicion(*G, v2), arco.distancia, arco.tipoconexion);
    else
        printf("Ya hay conexión entre estas ciudades\n");
}

//Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G) {
    tipovertice v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
   
    //Vértice origen del arco
    printf("Introduce nombre de la ciudad origen: ");
    scanf(" %[^\n]", v1.name);

    /*
    printf("Introduce región origen: ");
    scanf(" %[^\n]", v1.region);*/
    
    if (!existe_vertice(*G, v1)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v1.name, v1.region);
        return;
    }

    //Vértice destino del arco
    printf("Introduce nombre de la ciudad destino: ");
    scanf(" %[^\n]", v2.name);
    printf("Introduce región destino: ");
    scanf(" %[^\n]", v2.region);
    
    if (!existe_vertice(*G, v2)) {
        printf("La ciudad %s (%s) no existe en el grafo\n", v2.name, v2.region);
        return;
    }

    //Eliminación del arco
    if (son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        borrar_arco(G, posicion(*G, v1), posicion(*G, v2));
    else
        printf("No hay conexión entre estas ciudades\n");
}

//Opción i del menú, imprimir el grafo
//Función que imprime el grafo utilizando num_vertices para saber cuántos vértices tiene
//y array_vertices para recuperar el vector de vértices y recorrerlo

void imprimir_grafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo
    float dist; //distancia entre vértices
    char tipo; //tipo de conexión entre vértices

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;
    printf("A continuación tienes el mapa actual de conexiones de Poniente:\n");
    printf("        Simbolo -: conexion por tierra\n");
    printf("        Simbolo ~: conexion por mar\n\n");

    for (i = 0; i < N; i++) {
        //Imprimo el vértice en el nuevo formato
        printf("Ciudad(%d): %s - %s\n", i, VECTOR[i].name, VECTOR[i].region);
        //Chequeo sus arcos
        for (j = 0; j < N; j++) {
            if (son_adyacentes(G, i, j)) {
                //Calcular distancia y tipo de conexión del arco
                dist = distancia(G, i, j);
                tipo = tipoconexion(G, i, j);

                // Imprimir con el símbolo adecuado y el formato de espacios/flecha solicitado
                if (tipo == 't') {
                    // conexión por tierra: --> 
                    printf("        -->%s - %s (%.2f kms)\n", VECTOR[j].name, VECTOR[j].region, dist);
                } else if (tipo == 'm') {
                    // conexión por mar: ~~>
                    printf("        ~~>%s - %s (%.2f kms)\n", VECTOR[j].name, VECTOR[j].region, dist);
                } else {
                    // en caso de valor inesperado, mostrar de forma genérica
                    printf("        -->%s - %s (%.2f kms) [tipo desconocido]\n", VECTOR[j].name, VECTOR[j].region, dist);
                }
            }
        }
    } 
}

//Función para leer el archivo e inicializar el grafo
void leerArch(grafo *G, const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", filename);
        return;
    }

    char line[1024];
    int n;
    while (fgets(line, sizeof(line), file)) {
        tipoarco arco;
        tipovertice v1, v2;
        char tipo[32];
        n = sscanf(line, " %[^,],%[^,],%[^,],%[^,],%f,%s", v1.name, v1.region, v2.name, v2.region, &arco.distancia, tipo);
        
        if (n !=6){
            printf("Error al leer la línea: %s\n", line);
            continue;
        }
        
        // Comparar que el tipo de conexión es válido
        if (strcmp(tipo, "land") == 0)
            arco.tipoconexion = 't';
        else if (strcmp(tipo, "sea") == 0)
            arco.tipoconexion = 'm';
        else {
            printf("Tipo de conexión desconocido en la línea: %s\n", line);
            continue;
        }
        // Insertar vértices si no existen
        if (!existe_vertice(*G, v1)) insertar_vertice(G, v1);
        if (!existe_vertice(*G, v2)) insertar_vertice(G, v2);

        // Crear arco
        crear_arco(G, posicion(*G, v1), posicion(*G, v2), arco.distancia, arco.tipoconexion);
    } 
    fclose(file);
}

//Función para guardar el grafo en un archivo CSV
//Escribe cada arista solo una vez (grafo no dirigido): itera con j = i+1
void guardarArch(grafo G, const char* filename) {
    // Abrir archivo para escritura y comprobar errores
    FILE *file = fopen(filename, "w"); 
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir: %s\n", filename);
        return;
    }

    // Obtener el array de vértices y el número de vértices
    tipovertice *VECTOR = array_vertices(G);
    int N = num_vertices(G);

    int i, j; // Índices para recorrer los vértices del grafo

    // Recorrer todas las parejas (i,j) de vértices para escribir las aristas
    // Se recorre de tal forma que j > i para hacer que cada arco al ser no dirigido
    // se escriba solo una vez en el archivo
    for (i = 0; i < N; i++) {
        for (j = i + 1; j < N; j++) {
            // Comprobamos si existe conexión entre i y j
            // Si existe, escribimos la línea correspondiente en 
            // el archivo
            if (son_adyacentes(G, i, j)) {
                // Obtener distancia y tipo de conexión para poder
                // escribirlos en el archivo
                float dist = distancia(G, i, j);
                char tipo = tipoconexion(G, i, j);

                // Convertir el tipo de conexión al formato de cadena requerido
                char *tipocon;
                if (tipo == 't') tipocon = "land";
                else if (tipo == 'm') tipocon = "sea";
                else tipocon= "";

                // Formato: nombre,region,nombre,region,distancia,tipoconexion
                // Escribimos la distancia con 10 decimales
                fprintf(file, "%s,%s,%s,%s,%.10f,%s\n", VECTOR[i].name, VECTOR[i].region, VECTOR[j].name, VECTOR[j].region, dist, tipocon);
            }
        }
    }
    fclose(file);
}
