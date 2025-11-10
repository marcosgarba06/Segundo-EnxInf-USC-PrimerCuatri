//Código por Marcos García Blanco y Rosa María González Pérez

#include <stdio.h>                                   
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>

#define NUM_HILOS 16
#define NUM_ITERACIONES 1000000000 //Número de iteraciones que hará cada hilo creando numeros aleatorios
#define YIELD_CADA 1000 //Cada cuántas iteraciones hacer yield, se puede moficar para ver el efecto

//Definimos una estructura para almacenar toda la información de un hilo
typedef struct{    
    int id;                  //ID del hilo
    double tiempo;           //Tiempo de ejecución en microsegundos
    double pi_aproximado;    //Aproximación de π calculada
}InfoHilo;

InfoHilo info_hilos[NUM_HILOS];         //Array global para almacenar la información de todos los hilos, con número de elementos igual a la constante con el número de hilos que definimos previamente

//Declaramos los prototipos de las funciones que usaremos en main, cuyas definiciones se encuentran más adelante
int comparar_tiempos(const void* a, const void* b);       //Función que compara los tiempos que tardan dos hilos distintos en realizar las operaciones

void* hilo_func(void* arg);          //Función que ejecutan los hilos competitivos

void* hilo_generoso(void* arg);      //Función que ejecutan los hilos generosos

int main(int argc, char const *argv[]){

    pthread_t hilos[NUM_HILOS];         //Declaramos una conjunto de variables de tipo pthread_t(tipo de datos que representa un hilo) que tendrá tantos elementos como el número de hilos que queremos crear, que definimos en la constante NUM_HILOS.

    int id[NUM_HILOS];    //Definimos un array de ints que contendrá los ids de cada uno de los hilos, que asignaremos a los hilos para crear la semilla de generación aleatoria de puntos
    int res;        //Creamos una variable para comprobar que pthread_create y pthread_join ha funcionando correctamente

    printf("Creando hilos y calculando aproximaciones de π...\n");   //Informamos al usuario que se va a proceder a la creación de los hilos, que calcularán a través del método de Montecarlo la aproximación del número PI

    //CREACIÓN DE HILOS GENEROSOS Y COMPETITIVOS
    //Para crear los hilos generosos y no generosos alternadamente
    //se hace un if-else y si es par es generoso y si es impar no generoso
    //De esta forma tendremos el mismo numero de hilos generosos y no generosos
	
    //Creamos los hilos
    for(int i = 0; i < NUM_HILOS; i++){    //Tantas iteraciones como el número de hilos que queremos crear para crearlos todos
        //Para crear los hilos generosos y no generosos alternadamente
        //se hace un if-else y si es par es generoso y si es impar no generoso
        //De esta forma tendremos el mismo numero de hilos generosos y no generosos
        id[i] = i;    //Asignamos en el array de ids el id de cada hilo igual a su posición en el array de hilos
        if(i % 2 == 0){     //Si la posición del hilo en el array de hilos es par, creamos un hilo y le asignamos la función asociada a los hilos generosos
            res = pthread_create(&hilos[i], NULL, hilo_generoso, &id[i]);  //Pasamos el identificador del hilo i; lo creamos con los atributos por defecto; pasamos la función que tiene que ejecutar, hilo_generoso porque queremos que actúe como tal; y le pasamos como argumento de la función id[i], el ID que usamos para generar la semilla que cree puntos aleatorios en cada hilo. Guardamos la salida en la variable que creamos para ello, y comprobaremos que es igual a 0 si el hilo se ha creado correctamente

            //Verificamos si hubo un error al crear el hilo
            
            if(res != 0){    //Si pthread_create() devuelve algún valor que no sea 0, significa que hubo un error creando el hilo
                perror("Error al crear el hilo generoso");   //Informamos al usuario que se ha producido un error en la creación de uno de los hilos generosos
                exit(EXIT_FAILURE);     //Salimos del programa si se ha producido un error creando alguno de los hilos generosos
            }

        }else{    //Si la posición del hilo en el array de hilos no es par, entonces es impar, por lo que lo creamos y le asignamos la función asociada a los hilos competitivos
            res = pthread_create(&hilos[i], NULL, hilo_func, &id[i]);  //Pasamos el identificador del hilo i; lo creamos con los atributos por defecto; pasamos la función que tiene que ejecutar, hilo_func porque queremos que actúe como un hilo competitivo; y le pasamos como argumento de la función id[i], el ID que usamos para generar la semilla que cree puntos aleatorios en cada hilo. Guardamos la salida en la variable que creamos para ello, y comprobaremos que es igual a 0 si el hilo se ha creado correctamente

            //Verificamos si hubo un error al crear el hilo
            if(res != 0){      //Si pthread_create() devuelve algún valor que no sea 0, significa que hubo un error creando el hilo
                perror("Error al crear el hilo no generoso");   //Informamos al usuario que se ha producido un error en la creación de uno de los hilos no generosos
                exit(EXIT_FAILURE);      //Salimos del programa si se ha producido un error creando alguno de los hilos generosos
            }
        }
    }    

    //Esperamos a que todos los hilos terminen para poder terminar el programa principal
    for(int i = 0; i < NUM_HILOS; i++){     //Tantas iteraciones como el número de hilos que queremos recoger para esperarlos a todos
        res = pthread_join(hilos[i], NULL);    //El hilo principal espera al hilo correspondiente si todavía no ha acabado o lo recoge directamente; en los dos casos, este hilo principal que llama a pthread_join se queda bloqueado hasta que el hilo que quiere recoger ha acabado, y lo puede recoger y liberar todos sus datos. Guardamos la salida en la variable que creamos para ello, y comprobaremos que es igual a 0, si el hilo se ha creado correctamente

        //Verificamos si hubo un error al hacer join
        if(res != 0){   //Si pthread_join() devuelve algún valor que no sea 0, significa que hubo un error recogiendo el hilo
            perror("Error al hacer join del hilo");      //Informamos al usuario que se ha producido un error recogiendo a los hilos
            exit(EXIT_FAILURE);       //Salimos del programa si se ha producido un error recogiendo los hilos
        }
    }    
    printf("MAIN: Todos los hilos han terminado.\n");    //La función main del hilo principal informa de que todos los hilos han terminado

    //NOTA: Funcionamiento de Quick Sort
    //El algoritmo se basa en el principio de divide y vencerás:
    //Elegir un pivote de la lista que queremos ordenar
    //Dividir los elementos en dos sublistas: los menores al pivote y los mayores (o iguales) al pivote.
    //Ordenamos recursivamente cada sublista
    
    //Usamos qsort (Quick Sort) de la biblioteca estándar
    //Es más eficiente que Bubble Sort y ordena directamente el array de estructuras
    //void qsort(          //Argumentos de la fucnión qsort()
    //void *base,            //Puntero al primer elemento del conjunto que se quiere ordenar
    //size_t num,            //Número de elementos
    //size_t size,           //Tamaño (en bytes) de cada elemento
    //int (*compar)(const void *, const void *) //Función para comparar los elementos y ordenarlos
    //);

    qsort(info_hilos, NUM_HILOS, sizeof(InfoHilo), comparar_tiempos);    //Ordenamos los hilos según el tiempo que tardaron, usando quicksort y la función comparar_tiempos que definimos con este propósito

    //Imprimimos la cabecera de la tabla de resultados
    printf("=======================================================================\n");
    printf("| Hilo |      Tipo      | Tiempo (ms) | Aproximación de π |\n");
    printf("=======================================================================\n");
    
    //Recorremos el array ordenado e imprimimos cada hilo
    for(int i = 0; i < NUM_HILOS; i++){      //Recorremos todos los hilos
        //Ahora el array info_hilos ya está ordenado por tiempo
        int id_hilo = info_hilos[i].id;      //Obtenemos el id_hilo de la estructura asociada del tipo Infohilo para poder imprimir en la tabla si es generoso o es competitivo
        
        //Imprimimos la fila de la tabla para este hilo
        //Para saber el tipo, comprobamos si el ID es par (generoso) o impar (competitivo)
        printf("|  %2d  | %s |  %.3f   |    %.10f     |\n",
               id_hilo,                                                  //ID del hilo
               id_hilo % 2 == 0 ? "GENEROSO    " : "COMPETITIVO ",     //Tipo
               info_hilos[i].tiempo / 1000.0,                           //Tiempo en ms
               info_hilos[i].pi_aproximado);                            //Aproximación de π (PI)
    }
    
    printf("=======================================================================\n");

    exit (EXIT_SUCCESS);
}


//Función que ejecuta cada hilo no generoso
//Con esta función cada hilo calcula una aproximación de π usando el método de Montecarlo
//y mide el tiempo que tarda en hacerlo. NO cede el procesador voluntariamente con sched_yield
void* hilo_func(void* arg){
    int id = *((int*)arg); //Obtener el ID del hilo desde el argumento de pthread_create
    //Semilla para el generador de números aleatorios
    unsigned int seed = time(NULL) + id; //Usamos time(NULL) + id para que cada hilo tenga una semilla diferente
					 //time(NULL) devuelve la hora actual en segundos
    int i, dentro_circulo = 0;       //Definimos i para iterar las veces definidas y crear puntos, y dentro_circulo para almacenar cuántos puntos de los generados caen dentro del círculo
    
    struct timeval inicio, fin;      //Definimos dos estructuras de tipo timeval para guardar los tiempos con gettimeofday()
    
    //Llamamos dos veces consecutivas a gettimeofday() porque queremos calcular el overhead: cuánto tiempo consume una llamada a gettimeofday(). Esto se lo restaremos a los tiempos que midamos para que la llamada a gettimeofday() no distorsione los resultados
    gettimeofday(&inicio, NULL);
    gettimeofday(&fin, NULL);
    
    //Calculamos el overhead en microsegundos restando las dos medidas de tiempo
    //(segundos a microsegundos) + microsegundos
    double overhead = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec;
    
    gettimeofday(&inicio, NULL); //Reiniciamos el tiempo de inicio para la medición real; realizamos una nueva llamada a gettimeofday() para medir el tiempo que consume llevar a cabo el método de Montecarlo
    for (i = 0; i < NUM_ITERACIONES; i++)       //Realizamos los mil millones de iteraciones
    {
        
	//Cada iteracion crea un par de coordenadas aleatorias (x, y) entre 0 y 1
        //rand_r recibe el puntero a la semilla y la modifica en cada llamada mediante una fórmula que varía de acuerdo a varios parámetros
	double x = (double)rand_r(&seed) / RAND_MAX;     
        double y = (double)rand_r(&seed) / RAND_MAX;
        //se hace de esta forma usando rand_r ya que usando rand_r() cada hilo tiene su propia semilla
        //se divide entre RAND_MAX para normalizar el valor entre 0 y 1, ya que rand_r() devuelve un entero entre 0 y RAND_MAX
	//el mínimo de esta división será 0/RAND_MAX = 0 y el máximo será RAND_MAX/RAND_MAX = 1
        //RAND_MAX es una constante definida en stdlib.h que representa el valor máximo que puede devolver rand_r()
        
        //Comprobamos si el punto (x,y) está dentro del círculo de radio 1
        //La condición es: x² + y² ≤ 1
        if (x * x + y * y <= 1.0) {
            dentro_circulo++;  //Si está dentro, incrementamos el contador del número de puntos que caen dentro del círculo
        }    
    }

    //Calculamos la aproximación de π (PI) usando el método de Montecarlo
    //La fórmula es: π ≈ 4 * (puntos dentro del círculo / puntos totales)
    double pi_aproximado = 4.0 * dentro_circulo / NUM_ITERACIONES;

    gettimeofday(&fin, NULL); //Volvemos a llamar a gettimeofday() para obtener el tiempo en el final de la ejecución del hilo
    //Calculamos el tiempo total de ejecución en microsegundos
    double tiempo = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec - overhead;
    
    //Guardamos toda la información del hilo en la estructura
    info_hilos[id].id = id;
    info_hilos[id].tiempo = tiempo;
    info_hilos[id].pi_aproximado = pi_aproximado;
    
    //printf("Hilo %d: Aproximación de π = %.10f\n", id, pi_aproximado);
    pthread_exit(NULL); //salir del hilo
}

//Función que ejecuta cada hilo generoso
//Con esta función cada hilo calcula una aproximación de π usando el método de Montecarlo
//y mide el tiempo que tarda en hacerlo. Cede el procesador voluntariamente con sched_yield cada YIELD_CADA iteraciones
void* hilo_generoso(void* arg) {
    int id = *((int*)arg);   //Obtener el ID del hilo desde el argumento de pthread_create
    //Semilla para el generador de números aleatorios
    unsigned int seed = time(NULL) + id;    //Usamos time(NULL) + id para que cada hilo tenga una semilla diferente
                                         //time(NULL) devuelve la hora actual en segundos
    int i, dentro_circulo = 0;  //Definimos i para iterar las veces definidas y crear puntos, y dentro_circulo para almacenar cuántos puntos de los generados caen dentro del círculo
    
    struct timeval inicio, fin;  //Definimos dos estructuras de tipo timeval para guardar los tiempos con gettimeofday()
    
    //Llamamos dos veces consecutivas a gettimeofday() porque queremos calcular el overhead: cuánto tiempo consume una llamada a gettimeofday(). Esto se lo restaremos a los tiempos que midamos para que la llamada a gettimeofday() no distorsione los resultados
    gettimeofday(&inicio, NULL);
    gettimeofday(&fin, NULL);
    //Calculamos el overhead en microsegundos restando las dos medidas de tiempo
    //(segundos a microsegundos) + microsegundos
    double overhead = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec;

    gettimeofday(&inicio, NULL);       //Llamamos nuevamente a gettimeofday() para medir el tiempo que tarda el hilo
    for (i = 0; i < NUM_ITERACIONES; i++)
    {
        //Cada iteracion crea un par de coordenadas aleatorias (x, y) entre 0 y 1
        //rand_r recibe el puntero a la semilla y la modifica en cada llamada mediante una fórmula que varía de acuerdo a varios parámetros
        double x = (double)rand_r(&seed) / RAND_MAX; 
        double y = (double)rand_r(&seed) / RAND_MAX;
        //se hace de esta forma usando rand_r ya que usando rand_r() cada hilo tiene su propia semilla
        //se divide entre RAND_MAX para normalizar el valor entre 0 y 1, ya que rand_r() devuelve un entero entre 0 y RAND_MAX
        //el mínimo de esta división será 0/RAND_MAX = 0 y el máximo será RAND_MAX/RAND_MAX = 1
        //RAND_MAX es una constante definida en stdlib.h que representa el valor máximo que puede devolver rand_r()

	//Verificamos si cada punto cumple la condición (x² + y² <= 1.0)
        if (x * x + y * y <= 1.0) {
            dentro_circulo++;       //Si el punto verifica, sumamos 1 a la variable que cuenta el número de puntos que caen dentro del círculo
        }    

        //Comprobar si es una iteracion en la que se ha de ceder el procesador con sched_yield (cada vez que se hagan 1000 iteraciones = YIELD_CADA)
        if (i % YIELD_CADA == 0) {
            sched_yield(); //Ceder el procesador a otro hilo
        }
    }

    //Calculamos la aproximación de π (PI) usando el método de Montecarlo
    //La fórmula es: π ≈ 4 * (puntos dentro del círculo / puntos totales)
    double pi_aproximado = 4.0 * dentro_circulo / NUM_ITERACIONES;
    
    gettimeofday(&fin, NULL);    //Llamamos a gettimeofday() al finalizar el método

    //Calculamos el tiempo real restando el overhead
    double tiempo = (fin.tv_sec - inicio.tv_sec) * 1e6 + fin.tv_usec - inicio.tv_usec - overhead;
    
    //Guardamos toda la información del hilo en la estructura
    info_hilos[id].id = id;
    info_hilos[id].tiempo = tiempo;
    info_hilos[id].pi_aproximado = pi_aproximado;
    //printf("Hilo %d: Aproximación de π = %.10f\n", id, pi_aproximado);
    pthread_exit(NULL); //salir del hilo

}


//Función de comparación para qsort
//qsort necesita una función que compare dos elementos y devuelva:
//   - un número NEGATIVO si el primer elemento es MENOR que el segundo
//   - CERO si ambos elementos son IGUALES
//   - un número POSITIVO si el primer elemento es MAYOR que el segundo
int comparar_tiempos(const void* a, const void* b){   
    //qsort trabaja con punteros de tipo void* para ser flexible
    //Necesitamos convertirlos al tipo real de nuestros datos
    InfoHilo* hiloA = (InfoHilo*)a; //Definimos los elementos a comparar
    InfoHilo* hiloB = (InfoHilo*)b;

    // Comparamos los tiempos de ejecución
    if (hiloA->tiempo < hiloB->tiempo) //Primero menor que segundo
        return -1; 
    if (hiloA->tiempo > hiloB->tiempo) //Primero mayor que segundo
        return 1;
    else //Son iguales
        return 0;
}
