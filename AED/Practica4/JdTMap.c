#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h> // Para poder definir INFINITO como FLT_MAX
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


/*
 * Inicializa las matrices D (distancias) y P (vértices previos) para el
 * algoritmo de Floyd-Warshall según el pseudocódigo proporcionado.
 * 
 * Parámetros:
 *   G: grafo del que se extraerá la información de conexiones
 *   D: matriz de distancias mínimas (a inicializar)
 *   P: matriz de vértices previos (a inicializar)
 * 
 * Lógica de inicialización según el pseudocódigo:
 *   D(i,j) = A(i,j)   si A(i,j) ≠ 0 y i ≠ j (existe conexión directa)
 *   D(i,j) = 0        si i = j (distancia de un vértice a sí mismo)
 *   D(i,j) = ∞        en cualquier otro caso (no hay conexión directa)
 * 
 *   P(i,j) = i        si A(i,j) ≠ 0 y i ≠ j (el previo a j desde i es i mismo)
 *   P(i,j) = 0        en cualquier otro caso (no hay camino directo)
 */
void inicializar_floyd(grafo G, float D[][MAXVERTICES], int P[][MAXVERTICES]) {
    // Obtenemos el número total de vértices del grafo
    // Esto nos permite saber hasta dónde iterar en las matrices
    int N = num_vertices(G);
    

    int i, j;
    
    // Recorremos todas las filas de las matrices (cada vértice origen)
    for (i = 0; i < N; i++) {
        // Recorremos todas las columnas de las matrices (cada vértice destino)
        for (j = 0; j < N; j++) {
            
            // CASO 1: La diagonal principal (cuando i == j)
            // Un vértice tiene distancia 0 a sí mismo
            if (i == j) {
                D[i][j] = 0;      // Distancia de un vértice a sí mismo es 0
                P[i][j] = 0;      // No hay vértice previo (convenio: 0)
            }
            
            // CASO 2: Existe una conexión directa entre i y j (son adyacentes)
            // Debemos copiar la distancia de la matriz de adyacencia del grafo
            else if (son_adyacentes(G, i, j)) {
                // Copiamos la distancia real del arco entre i y j
                D[i][j] = distancia(G, i, j);
                
                // El vértice previo a j viniendo desde i es i mismo
                // Esto significa que para ir de i a j, el paso anterior es i
                P[i][j] = i;
            }
            
            // CASO 3: No existe conexión directa entre i y j (no son adyacentes)
            // Inicializamos con "infinito" para indicar que no hay camino directo
            else {
                // FLT_MAX representa el infinito en números flotantes
                // Esto indica que inicialmente no conocemos un camino de i a j
                D[i][j] = FLT_MAX;
                
                // No hay vértice previo porque no existe camino directo
                P[i][j] = 0;
            }
        }
    }
    // Al finalizar esta función, tenemos:
    // - Matriz D: contiene las distancias directas entre vértices adyacentes,
    //             0 en la diagonal, e infinito donde no hay conexión directa
    // - Matriz P: contiene el vértice previo para cada arco directo,
    //             lo que nos permitirá reconstruir los caminos más adelante
}

/*
 * Calcula las distancias mínimas entre todos los pares de vértices usando
 * el algoritmo de Floyd-Warshall. Modifica las matrices D y P.
 * 
 * Parámetros:
 *   G: grafo con la información de los vértices
 *   D: matriz de distancias (ya inicializada en el Paso 1)
 *   P: matriz de vértices previos (ya inicializada en el Paso 1)
 * 
 * Lógica según el pseudocódigo:
 *   Para cada vértice k (vértice intermedio):
 *     Para cada vértice i (origen):
 *       Para cada vértice j (destino):
 *         Si el camino i->k->j es más corto que i->j directo:
 *           Actualizar D(i,j) con la nueva distancia más corta
 *           Actualizar P(i,j) con el vértice previo de k->j
 */
void floyd_warshall(grafo G, float D[][MAXVERTICES], int P[][MAXVERTICES]) {
    // Obtenemos el número total de vértices del grafo
    int N = num_vertices(G);
    
    // Variables para los tres bucles anidados
    int k, i, j;
    
    // =========================================================================
    // TRIPLE BUCLE ANIDADO: Corazón del algoritmo de Floyd-Warshall
    // =========================================================================
    
    // Iteramos sobre cada posible vértice INTERMEDIO k
    // En cada iteración construimos la matriz Dk, que considera caminos
    // que pueden pasar por los vértices {0, 1, 2, ..., k}
    for (k = 0; k < N; k++) {
        
        // Iteramos sobre cada posible vértice ORIGEN i
        for (i = 0; i < N; i++) {
            
            // Iteramos sobre cada posible vértice DESTINO j
            for (j = 0; j < N; j++) {
                // Comparamos dos opciones:
                //   Opción A: Ir directamente de i a j (distancia actual D[i][j])
                //   Opción B: Ir de i a k, y luego de k a j (D[i][k] + D[k][j])
                //
                // Si la Opción B es mejor, actualizamos las matrices
                
                if (D[i][j] > D[i][k] + D[k][j]) {
                    // Guardar la nueva distancia más corta
                    // La distancia de i a j ahora es la suma de:
                    //   - Distancia de i a k (D[i][k])
                    //   - Distancia de k a j (D[k][j])
                    D[i][j] = D[i][k] + D[k][j];
                    
                    // Guardar el vértice previo correcto
                    // Para llegar a j desde i (pasando por k), el vértice
                    // previo a j es el mismo que usaríamos para ir de k a j
                    // Es decir, copiamos P[k][j] en P[i][j]
                    P[i][j] = P[k][j];
                }
            }
        }
    }
    
    // Al terminar los tres bucles, las matrices D y P contienen:
    // Matriz D: Las distancias MÍNIMAS entre TODOS los pares de vértices
    //           Considera todos los caminos posibles en el grafo
    // Matriz P: Los vértices previos que permiten reconstruir el camino
    //           más corto entre cualquier par de vértices
}


/*
 * Función: imprimir_camino
 * ------------------------
 * Imprime el camino más corto entre un vértice origen y un vértice destino
 * usando la matriz de vértices previos P calculada por Floyd-Warshall.
 * 
 * Esta es una función RECURSIVA que reconstruye el camino de atrás hacia adelante.
 * 
 * Parámetros:
 *   G: grafo (para obtener información de los vértices y las conexiones)
 *   P: matriz de vértices previos (calculada por Floyd-Warshall)
 *   origen: índice del vértice origen
 *   destino: índice del vértice destino actual
 */
void imprimir_camino(grafo G, int P[][MAXVERTICES], int origen, int destino) {
    // Obtenemos el array de vértices para poder mostrar nombres de ciudades
    tipovertice *VECTOR = array_vertices(G);
    
    
    // Si origen == destino, solo imprimimos el destino
    // Ocurre cuando llegamos al principio del camino en la recursión
    
    // Si origen es distinto destino
    if (origen != destino) {
        //  Llamada recursiva para imprimir el camino hasta el previo
        // P[origen][destino] nos da el vértice previo a 'destino' en el
        // camino óptimo desde 'origen'
        //
        // Ejemplo: Si vamos de Winterfell (0) a King's Landing (5)
        // y P[0][5] = 3 (Harrenhal), entonces primero imprimimos
        // el camino de Winterfell a Harrenhal
        imprimir_camino(G, P, origen, P[origen][destino]);
        
        // PASO 2: Determinar el tipo de conexión para imprimir el símbolo correcto
        // Obtenemos el vértice previo a destino
        int previo = P[origen][destino];
        
        // Obtenemos el tipo de conexión entre el previo y el destino
        char tipo = tipoconexion(G, previo, destino);
        
        // Imprimimos la flecha según el tipo de conexión:
        // 't' (tierra) -> "--->"
        // 'm' (mar)    -> "~~~>"
        if (tipo == 't') {
            printf("--->");
        } else if (tipo == 'm') {
            printf("~~~>");
        } else {
            printf("-->"); // Caso por defecto (no debería ocurrir)
        }
    }
    
    // =========================================================================
    // IMPRESIÓN DEL VÉRTICE DESTINO
    // =========================================================================
    // Después de imprimir todo el camino previo (o si estamos en el caso base),
    // imprimimos el nombre de la ciudad destino actual
    printf("%s", VECTOR[destino].name);
    
    // =========================================================================
    // CÓMO FUNCIONA LA RECURSIÓN (Ejemplo):
    // =========================================================================
    // Supongamos el camino: Winterfell -> Moat Cailin -> Harrenhal -> King's Landing
    // Con índices:           0             1              2             3
    //
    // Llamada inicial: imprimir_camino(G, P, 0, 3)
    //   P[0][3] = 2, entonces llamamos: imprimir_camino(G, P, 0, 2)
    //     P[0][2] = 1, entonces llamamos: imprimir_camino(G, P, 0, 1)
    //       P[0][1] = 0, entonces llamamos: imprimir_camino(G, P, 0, 0)
    //         origen == destino, imprimimos: "Winterfell"
    //       Volvemos, imprimimos: "--->" (o "~~~>") y "Moat Cailin"
    //     Volvemos, imprimimos: "--->" (o "~~~>") y "Harrenhal"
    //   Volvemos, imprimimos: "--->" (o "~~~>") y "King's Landing"
    //
    // Resultado: Winterfell--->Moat Cailin--->Harrenhal--->King's Landing
    // =========================================================================
}


//==============================================================================
// FUNCIÓN DE MENÚ: BUSCAR LA RUTA MÁS CORTA (OPCIÓN E)
//==============================================================================

/*
 * Función: buscar_ruta_mas_corta
 * ------------------------------
 * Implementa la opción 'e' del menú: buscar la ruta más corta entre dos ciudades.
 * 
 * Esta función:
 * 1. Pide al usuario las ciudades origen y destino
 * 2. Verifica que ambas ciudades existen
 * 3. Inicializa las matrices para Floyd-Warshall
 * 4. Ejecuta el algoritmo de Floyd-Warshall
 * 5. Verifica que existe un camino entre origen y destino
 * 6. Imprime la ruta y la distancia total
 * 
 * Parámetros:
 *   G: puntero al grafo
 */
void buscar_ruta_mas_corta(grafo *G) {
    // Declaración de variables
    tipovertice origen, destino;  // Ciudades origen y destino
    int pos_origen, pos_destino;  // Posiciones en el array de vértices
    
    // Matrices para Floyd-Warshall
    float D[MAXVERTICES][MAXVERTICES];  // Matriz de distancias mínimas
    int P[MAXVERTICES][MAXVERTICES];    // Matriz de vértices previos
    
    // =========================================================================
    // PASO 1: Solicitar ciudad ORIGEN al usuario
    // =========================================================================
    printf("\nEspecifica el nombre de la ciudad de origen: ");
    scanf(" %[^\n]", origen.name);  // Leemos el nombre completo (con espacios)
    
    // Verificar que la ciudad origen existe en el grafo
    // La función posicion() devuelve -1 si no encuentra el vértice
    pos_origen = posicion(*G, origen);
    if (pos_origen == -1) {
        printf("Error: La ciudad '%s' no existe en el mapa.\n", origen.name);
        return;  // Salimos de la función si la ciudad no existe
    }
    
    // =========================================================================
    // PASO 2: Solicitar ciudad DESTINO al usuario
    // =========================================================================
    printf("Especifica el nombre de la ciudad de destino: ");
    scanf(" %[^\n]", destino.name);
    
    // Verificar que la ciudad destino existe en el grafo
    pos_destino = posicion(*G, destino);
    if (pos_destino == -1) {
        printf("Error: La ciudad '%s' no existe en el mapa.\n", destino.name);
        return;
    }
    
    // =========================================================================
    // PASO 3: Ejecutar el algoritmo de Floyd-Warshall
    // =========================================================================
    // Primero inicializamos las matrices D y P (Paso 1)
    inicializar_floyd(*G, D, P);
    
    // Luego calculamos las distancias mínimas (Paso 2)
    floyd_warshall(*G, D, P);
    
    // =========================================================================
    // PASO 4: Verificar que existe un camino entre origen y destino
    // =========================================================================
    // Si D[pos_origen][pos_destino] es infinito, no existe camino
    if (D[pos_origen][pos_destino] == FLT_MAX) {
        printf("\nNo existe ruta entre %s y %s.\n", origen.name, destino.name);
        return;
    }
    
    // =========================================================================
    // PASO 5: Imprimir la ruta y la distancia total
    // =========================================================================
    printf("\nRuta: ");
    
    // Llamamos a la función recursiva para imprimir el camino (Paso 3)
    imprimir_camino(*G, P, pos_origen, pos_destino);
    
    // Imprimimos la distancia total con 2 decimales
    printf("\nLa distancia total desde %s hasta %s es de %.2f kms\n",
           origen.name, destino.name, D[pos_origen][pos_destino]);
}


