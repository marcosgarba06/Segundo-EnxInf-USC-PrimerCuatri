#include <stdio.h>                                   
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>

#define NUM_HILOS 8 // Número total de hilos debe ser par para tener igual número de generosos y no generosos y se debe ajustar segun la capacidad de la CPU del sistema
#define NUM_ITERACIONES 1000000000 // Número de iteraciones que hará cada hilo creando numeros aleatorios
#define YIELD_CADA 1000 // Cada cuántas iteraciones hacer yield, se puede moficar para ver el efecto

// Estructura para almacenar toda la información de un hilo
typedef struct {
    int id;                  // ID del hilo
    double tiempo;           // Tiempo de ejecución en microsegundos
    double pi_aproximado;    // Aproximación de π calculada
} InfoHilo;

// Array global para almacenar la información de todos los hilos
InfoHilo info_hilos[NUM_HILOS];

int comparar_tiempos(const void* a, const void* b);

void* hilo_func(void* arg);

void* hilo_generoso(void* arg);

int main(int argc, char const *argv[])
{
    pthread_t hilos[NUM_HILOS];
    int id[NUM_HILOS];
    int res;

    printf("Creando hilos y calculando aproximaciones de π...\n");
    // Crear múltiples hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        // Para crear los hilos generosos y no generosos alternadamente
        // se hace un if-else y si es par es generoso y si es impar no generoso
        // De esta forma tendremos el mismo numero de hilos generosos y no generosos
        id[i] = i;
        if (i % 2 == 0) {
            res = pthread_create(&hilos[i], NULL, hilo_generoso, &id[i]);
            // Verificar si hubo un error al crear el hilo
            
            if (res != 0) {
                perror("Error al crear el hilo generoso");
                exit(EXIT_FAILURE);
            }

        } else {
            res = pthread_create(&hilos[i], NULL, hilo_func, &id[i]);

            // Verificar si hubo un error al crear el hilo
            if (res != 0) {
                perror("Error al crear el hilo no generoso");
                exit(EXIT_FAILURE);
            }
        }
    }    

    // Esperar a que todos los hilos terminenn para poder terminar el programa principal
    for (int i = 0; i < NUM_HILOS; i++) {
        res = pthread_join(hilos[i], NULL);
        // Verificar si hubo un error al hacer join
        if (res != 0) {
            perror("Error al hacer join del hilo");
            exit(EXIT_FAILURE);
        }
    }    
    printf("MAIN: Todos los hilos han terminado.\n");

    // Usamos qsort (Quick Sort) de la biblioteca estándar
    // Es más eficiente que Bubble Sort y ordena directamente el array de estructuras
    qsort(info_hilos, NUM_HILOS, sizeof(InfoHilo), comparar_tiempos);

    printf("=======================================================================\n");
    printf("| Hilo |      Tipo      | Tiempo (ms) | Aproximación de π |\n");
    printf("=======================================================================\n");
    
    // Recorremos el array ordenado e imprimimos cada hilo
    for (int i = 0; i < NUM_HILOS; i++) {
        // Ahora el array info_hilos ya está ordenado por tiempo
        int id_hilo = info_hilos[i].id;
        
        // Imprimimos la fila de la tabla para este hilo
        // Para saber el tipo, comprobamos si el ID es par (generoso) o impar (competitivo)
        printf("|  %2d  | %s |  %.3f   |    %.10f     |\n",
               id_hilo,                                                  // ID del hilo
               id_hilo % 2 == 0 ? "GENEROSO    " : "COMPETITIVO ",     // Tipo
               info_hilos[i].tiempo / 1000.0,                           // Tiempo en ms
               info_hilos[i].pi_aproximado);                            // Aproximación de π
    }
    
    printf("=======================================================================\n");

    exit (EXIT_SUCCESS);
}


// Función que ejecuta cada hilo no generoso
// Con esta función cada hilo calcula una aproximación de π usando el método de Montecarlo
// y mide el tiempo que tarda en hacerlo. NO cede el procesador voluntariamente con sched yield
void* hilo_func(void* arg) {
    int id = *((int*)arg); // Obtener el ID del hilo desde el argumento de pthread_create
    // Semilla para el generador de números aleatorios
    unsigned int seed = time(NULL) + id; // Usamos time(NULL) + id para que cada hilo tenga una semilla diferente
    int i, dentro_circulo = 0;
    
    struct timeval inicio, fin;
    // Llamamos dos veces consecutivas para medir cuánto tarda la función
    gettimeofday(&inicio, NULL);
    gettimeofday(&fin, NULL);
    
    // Calculamos el overhead en microsegundos
    // (segundos a microsegundos) + microsegundos
    double overhead = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec;
    
    gettimeofday(&inicio, NULL); // Reiniciamos el tiempo de inicio para la medición real
    for (i = 0; i < NUM_ITERACIONES; i++)
    {
        // Cada iteracion crea un par de coordenadas aleatorias (x, y) entre 0 y 1
        double x = (double)rand_r(&seed) / RAND_MAX; 
        double y = (double)rand_r(&seed) / RAND_MAX;
        // se hace de esta forma usando rand_r ya que usando rand_r() cada hilo tiene su propia semilla
        // se divide entre RAND_MAX para normalizar el valor entre 0 y 1, ya que rand_r() devuelve un entero entre 0 y RAND_MAX
        // RAND_MAX es una constante definida en stdlib.h que representa el valor máximo que puede devolver rand_r()
        
        // Comprobamos si el punto (x,y) está dentro del círculo de radio 1
        // La condición es: x² + y² ≤ 1
        if (x * x + y * y <= 1.0) {
            dentro_circulo++;  // Si está dentro, incrementamos el contador
        }    
    }

    // Calculamos la aproximación de π usando el método de Montecarlo
    // La fórmula es: π ≈ 4 * (puntos dentro del círculo / puntos totales)
    double pi_aproximado = 4.0 * dentro_circulo / NUM_ITERACIONES;

    gettimeofday(&fin, NULL); // Tiempo de fin de la ejecución del hilo
    // Calculamos el tiempo total de ejecución en microsegundos
    double tiempo = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec - overhead;
    
    // Guardamos toda la información del hilo en la estructura
    info_hilos[id].id = id;
    info_hilos[id].tiempo = tiempo;
    info_hilos[id].pi_aproximado = pi_aproximado;
    
   //printf("Hilo %d: Aproximación de π = %.10f\n", id, pi_aproximado);
    pthread_exit(NULL); //salir del hilo
}

// Función que ejecuta cada hilo generoso
// Con esta función cada hilo calcula una aproximación de π usando el método de Montecarlo
// y mide el tiempo que tarda en hacerlo. Cede el procesador voluntariamente con sched_yield cada YIELD_CADA iteraciones
void* hilo_generoso(void* arg) {
    int id = *((int*)arg);

    unsigned int seed = time(NULL) + id; 

    int i, dentro_circulo = 0;
    
    struct timeval inicio, fin;
    
    gettimeofday(&inicio, NULL);
    gettimeofday(&fin, NULL);
    double overhead = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec;

    gettimeofday(&inicio, NULL);
    for (i = 0; i < NUM_ITERACIONES; i++)
    {
        // Cada iteracion crea un par de coordenadas aleatorias (x, y) entre 0 y 1
        double x = (double)rand_r(&seed) / RAND_MAX; 
        double y = (double)rand_r(&seed) / RAND_MAX;
       
        if (x * x + y * y <= 1.0) {
            dentro_circulo++;  
        }    

        //Comprobar si es una iteracion en la que se ha de ceder el procesador con sched_yield
        if (i % YIELD_CADA == 0) {
            sched_yield(); // Ceder el procesador a otro hilo
        }
    }

    
    double pi_aproximado = 4.0 * dentro_circulo / NUM_ITERACIONES;
    
    gettimeofday(&fin, NULL);

    // Calculamos el tiempo real restando el overhead
    double tiempo = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec - overhead;
    
    // Guardamos toda la información del hilo en la estructura
    info_hilos[id].id = id;
    info_hilos[id].tiempo = tiempo;
    info_hilos[id].pi_aproximado = pi_aproximado;
    //printf("Hilo %d: Aproximación de π = %.10f\n", id, pi_aproximado);
    pthread_exit(NULL); //salir del hilo

}


// Función de comparación para qsort
// qsort necesita una función que compare dos elementos y devuelva:
//   - un número NEGATIVO si el primer elemento es MENOR que el segundo
//   - CERO si ambos elementos son IGUALES
//   - un número POSITIVO si el primer elemento es MAYOR que el segundo
int comparar_tiempos(const void* a, const void* b) {
    // qsort trabaja con punteros de tipo void* para ser flexible
    // Necesitamos convertirlos al tipo real de nuestros datos
    InfoHilo* hiloA = (InfoHilo*)a; //Definimos los elementos a comparar
    InfoHilo* hiloB = (InfoHilo*)b;

    // Comparamos los tiempos de ejecución
    if (hiloA->tiempo < hiloB->tiempo) // Primero menor que segundo
        return -1; 
    if (hiloA->tiempo > hiloB->tiempo) // Primero mayor que segundo
        return 1;
    else // Son iguales
        return 0;
}