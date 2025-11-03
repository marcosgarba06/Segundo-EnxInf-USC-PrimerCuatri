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
 * Inicializa las matrices D (distancias o tiempos) y P (vértices previos) para el
 * algoritmo de Floyd-Warshall según el pseudocódigo proporcionado.
 * 
 * MODIFICACIÓN: Esta función ahora puede inicializar la matriz para optimizar
 * tanto DISTANCIA como TIEMPO según los parámetros recibidos.
 * 
 * Parámetros:
 *   G: grafo del que se extraerá la información de conexiones
 *   D: matriz de distancias/tiempos mínimos (a inicializar)
 *   P: matriz de vértices previos (a inicializar)
 *   optimizar_tiempo: flag que indica qué optimizar:
 *                     0 = optimizar DISTANCIA (usar distancias directamente)
 *                     1 = optimizar TIEMPO (calcular tiempo = distancia/velocidad)
 *   vel_tierra: velocidad de desplazamiento por tierra en km/h
 *               (solo se usa si optimizar_tiempo == 1)
 *   vel_mar: velocidad de desplazamiento por mar en km/h
 *            (solo se usa si optimizar_tiempo == 1)
 * 
 * Lógica de inicialización según el pseudocódigo:
 *   Si optimizar_tiempo == 0 (DISTANCIA):
 *     D(i,j) = distancia(i,j)   si A(i,j) ≠ 0 y i ≠ j (existe conexión directa)
 *   
 *   Si optimizar_tiempo == 1 (TIEMPO):
 *     D(i,j) = distancia(i,j) / velocidad   si A(i,j) ≠ 0 y i ≠ j
 *              donde velocidad = vel_tierra si el tipo de conexión es 't'
 *              donde velocidad = vel_mar si el tipo de conexión es 'm'
 *   
 *   En ambos casos:
 *     D(i,j) = 0        si i = j (coste de un vértice a sí mismo es 0)
 *     D(i,j) = ∞        en cualquier otro caso (no hay conexión directa)
 *   
 *     P(i,j) = i        si A(i,j) ≠ 0 y i ≠ j (el previo a j desde i es i mismo)
 *     P(i,j) = 0        en cualquier otro caso (no hay camino directo)
 */
void inicializar_floyd(grafo G, float D[][MAXVERTICES], int P[][MAXVERTICES],
                       int optimizar_tiempo, float vel_tierra, float vel_mar) {
    // Obtenemos el número total de vértices del grafo
    // Esto nos permite saber hasta dónde iterar en las matrices
    int N = num_vertices(G);
    
    int i, j;
    
    // Recorremos todas las filas de las matrices (cada vértice origen)
    for (i = 0; i < N; i++) {
        // Recorremos todas las columnas de las matrices (cada vértice destino)
        for (j = 0; j < N; j++) {
            
            // CASO 1: La diagonal principal (cuando i == j)
            // Un vértice tiene distancia/tiempo 0 a sí mismo
            if (i == j) {
                D[i][j] = 0;      // Coste de un vértice a sí mismo es 0
                P[i][j] = 0;      // No hay vértice previo (convenio: 0)
            }
            
            // CASO 2: Existe una conexión directa entre i y j (son adyacentes)
            // Debemos calcular el coste según si optimizamos distancia o tiempo
            else if (son_adyacentes(G, i, j)) {
                // Obtenemos la distancia en km del arco
                float dist = distancia(G, i, j);
                
                // Dependiendo del modo de optimización, calculamos el coste
                if (optimizar_tiempo) {
                    // MODO TIEMPO: Calculamos tiempo = distancia / velocidad
                    // La velocidad depende del tipo de conexión (tierra o mar)
                    char tipo = tipoconexion(G, i, j);
                    
                    // Si es tierra ('t'), usamos velocidad por tierra
                    // Si es mar ('m'), usamos velocidad por mar
                    if (tipo == 't') {
                        D[i][j] = dist / vel_tierra;  // Tiempo en horas por tierra
                    } else if (tipo == 'm') {
                        D[i][j] = dist / vel_mar;     // Tiempo en horas por mar
                    } else {
                        // Tipo desconocido, ponemos infinito
                        D[i][j] = FLT_MAX;
                    }
                } else {
                    // MODO DISTANCIA: Usamos directamente la distancia
                    D[i][j] = dist;
                }
                
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
    // - Matriz D: contiene las distancias/tiempos directos entre vértices adyacentes,
    //             0 en la diagonal, e infinito donde no hay conexión directa
    // - Matriz P: contiene el vértice previo para cada arco directo,
    //             lo que nos permitirá reconstruir los caminos más adelante
}

/*
 * Calcula las distancias/tiempos mínimos entre todos los pares de vértices usando
 * el algoritmo de Floyd-Warshall. Modifica las matrices D y P.
 * 
 * NOTA: Esta función funciona igual tanto para distancias como para tiempos,
 * ya que solo realiza operaciones de comparación y suma. La diferencia está
 * en cómo se inicializó la matriz D en la función inicializar_floyd.
 * 
 * Parámetros:
 *   G: grafo con la información de los vértices
 *   D: matriz de distancias/tiempos (ya inicializada en el Paso 1)
 *   P: matriz de vértices previos (ya inicializada en el Paso 1)
 * 
 * Lógica según el pseudocódigo:
 *   Para cada vértice k (vértice intermedio):
 *     Para cada vértice i (origen):
 *       Para cada vértice j (destino):
 *         Si el camino i->k->j es más corto/rápido que i->j directo:
 *           Actualizar D(i,j) con el nuevo coste más bajo
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
                //   Opción A: Ir directamente de i a j (coste actual D[i][j])
                //   Opción B: Ir de i a k, y luego de k a j (D[i][k] + D[k][j])
                //
                // Si la Opción B es mejor, actualizamos las matrices
                
                if (D[i][j] > D[i][k] + D[k][j]) {
                    // Guardar el nuevo coste más bajo
                    // El coste de i a j ahora es la suma de:
                    //   - Coste de i a k (D[i][k])
                    //   - Coste de k a j (D[k][j])
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
    // Matriz D: Los costes MÍNIMOS entre TODOS los pares de vértices
    //           Considera todos los caminos posibles en el grafo
    // Matriz P: Los vértices previos que permiten reconstruir el camino
    //           más corto/rápido entre cualquier par de vértices
}


/*
 * Función: imprimir_camino
 * ------------------------
 * Imprime el camino más corto/rápido entre un vértice origen y un vértice destino
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
    
    if (origen != destino) {
        imprimir_camino(G, P, origen, P[origen][destino]);
        
        // Determinar el tipo de conexión para imprimir el símbolo correcto
        // Obtenemos el vértice previo a destino
        int previo = P[origen][destino];
        
        // Obtenemos el tipo de conexión entre el previo y el destino
        char tipo = tipoconexion(G, previo, destino);
        
        // Imprimimos la flecha según el tipo de conexión:
        if (tipo == 't') {
            printf("--->");
        } else if (tipo == 'm') {
            printf("~~~>");
        } else {
            printf("-->"); 
        }
    }
    
    // Después de imprimir todo el camino previo imprimimos el nombre de la ciudad destino actual
    printf("%s", VECTOR[destino].name);
}

/*
 * Función que busca la ruta óptima entre dos ciudades.
 * Puede optimizar por DISTANCIA o por TIEMPO según el parámetro recibido.
 * 
 * Parámetros:
 *   G: puntero al grafo
 *   optimizar_tiempo: flag que indica que optimizar:
 *                     0 = optimizar DISTANCIA (ruta más corta)
 *                     1 = optimizar TIEMPO (ruta más rápida)
 */
void buscar_ruta_optima(grafo *G, int optimizar_tiempo) {
    tipovertice origen, destino;  // Ciudades origen y destino
    int pos_origen, pos_destino;  // Posiciones en el array de vértices
    
    // Matrices para Floyd-Warshall
    float D[MAXVERTICES][MAXVERTICES];  // Matriz de costes mínimos
    int P[MAXVERTICES][MAXVERTICES];    // Matriz de vértices previos
    
    // Variables para velocidades
    float vel_tierra, vel_mar;
    char medio;  // Medio de transporte elegido por el usuario

    printf("\nEspecifica el nombre de la ciudad de origen: ");
    scanf(" %[^\n]", origen.name);
    
    // Verificar que la ciudad origen existe en el grafo
    // La función posicion() devuelve -1 si no encuentra el vértice
    pos_origen = posicion(*G, origen);
    if (pos_origen == -1) {
        printf("Error: La ciudad '%s' no existe en el mapa.\n", origen.name);
        return;
    }
    
    printf("Especifica el nombre de la ciudad de destino: ");
    scanf(" %[^\n]", destino.name);
    
    // Verificar que la ciudad destino existe en el grafo
    pos_destino = posicion(*G, destino);
    if (pos_destino == -1) {
        printf("Error: La ciudad '%s' no existe en el mapa.\n", destino.name);
        return;
    }
    
    //  Si optimizamos TIEMPO, preguntar el medio de transporte
    if (optimizar_tiempo) {
        printf("\nMedio de transporte:\n");
        printf("  c - Caballo (por tierra y/o mar)\n");
        printf("  d - Dragón\n");
        printf("Opción: ");
        scanf(" %c", &medio);
        
        // Configurar velocidades según el medio elegido
        if (medio == 'c' || medio == 'C') {
            // CABALLO: velocidades diferentes para tierra y mar
            vel_tierra = 5.5;     // km/h a caballo por tierra
            vel_mar = 11.25;      // km/h en barco por mar
        } else if (medio == 'd' || medio == 'D') {
            // DRAGÓN: misma velocidad por tierra y mar
            vel_tierra = 80.0;    // km/h en dragón
            vel_mar = 80.0;       // km/h en dragón
        } else {
            printf("Opción no válida.\n");
            return;
        }
    }
    
    // Ejecutar el algoritmo de Floyd-Warshall
    // Primero inicializamos las matrices D y P
    inicializar_floyd(*G, D, P, optimizar_tiempo, vel_tierra, vel_mar);
    
    // Luego calculamos los costes mínimos con Floyd-Warshall
    // El algoritmo funciona igual para distancias y tiempos
    floyd_warshall(*G, D, P);
    

    // Verificar que existe un camino entre origen y destino
    // Si D[pos_origen][pos_destino] es infinito, no existe camino
    if (D[pos_origen][pos_destino] == FLT_MAX) {
        printf("\nNo existe ruta entre %s y %s.\n", origen.name, destino.name);
        return;
    }
    
    
    // Imprimir la ruta y el coste total

    if (optimizar_tiempo) {
        printf("\nRuta más rápida: ");
    } else {
        printf("\nRuta más corta: ");
    }
    
    // Llamamos a la función recursiva para imprimir el camino
    imprimir_camino(*G, P, pos_origen, pos_destino);
    
    // Imprimimos el coste total con el formato adecuado
    if (optimizar_tiempo) {
        // Si optimizamos tiempo, mostramos el resultado en horas
        printf("\nEl tiempo total desde %s hasta %s es de %.2f horas\n",
               origen.name, destino.name, D[pos_origen][pos_destino]);
    } else {
        // Si optimizamos distancia, mostramos el resultado en kilómetros
        printf("\nLa distancia total desde %s hasta %s es de %.2f kms\n",
               origen.name, destino.name, D[pos_origen][pos_destino]);
    }
}

/*
 * Opción 'e' del menú: buscar la ruta más corta entre dos ciudades.
 * 
 * Es una función que llama a la función
 * buscar_ruta_optima con el parámetro optimizar_tiempo = 0.
 * De esta forma el codigo del menu es mas claro.
 * 
 */
void buscar_ruta_mas_corta(grafo *G) {
    buscar_ruta_optima(G, 0);
}

/*
 * Opción 'f' del menú: buscar la ruta más rápida entre dos ciudades.
 * 
 * Esta es una función que llama a la función
 * buscar_ruta_optima con el parámetro optimizar_tiempo = 1.
 */
void buscar_ruta_mas_rapida(grafo *G) {
    buscar_ruta_optima(G, 1);
}
