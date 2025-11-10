//Código por Marcos García Blanco y Rosa María González Pérez

/*
=== Observaciones sobre los casos con más hilos que cores ===
Datos:
    N = 1e9 iteraciones
    CPU con 8 núcleos físicos y 16 hilos físicos

Resultados:
    Tiempo paralelo con H=24 : 2.074 s
    Tiempo paralelo con H=16 : 1.978 s
    Tiempo paralelo con H=8 : 2.557 s
    Tiempo secuencial con H = 24: 15.793 s
    Tiempo secuencial con H = 16: 15.603 s
    Tiempo secuencial con H = 8: 15.607 s
    Ahorro con H=24 : 7.61
    Ahorro con H=16: 7.89
    Ahorro con H = 8: 6.104

Análisis:
    - Cuando el número de hilos es igual al número de cores físicos (H = 8),
      no se aprovechan todos los recursos si se dispone de más de un
      hilo por núcleo (como es el caso), lo que hace que el tiempo no sea
      el óptimo. El paralelismo que se puede hacer será más lento respecto al de
      un número de hilos superior al número de cores, como 16 o 24.
    - Cuando el número de hilos es superior al número de hilos físicos (H = 24),
      aunque la carga de trabajo total es la misma (N = 1e9),
      el sistema operativo debe repartir el tiempo de CPU entre los hilos
      mediante cambios de contexto. Cada cambio de contexto tiene un coste
      adicional (guardar y restaurar el estado de un hilo), lo que introduce
      overhead y reduce la eficiencia.
    - En consecuencia, los hilos no se ejecutan realmente en paralelo
      cuando el número de hilos es superior a la capacidad física:
      varios compiten por los mismos núcleos físicos, y el beneficio del
      paralelismo se satura o incluso empeora. Este efecto se aprecia en
      el tiempo total: con 24 hilos (más que los 16 que la CPU puede
      manejar en paralelo) el programa tarda más que con 16 hilos.
    - Cuando el número de hilos es igual al número de hilos físicos (H = 16), estamos
      en el caso óptimo: el mayor paralelismo posible sin desaprovechar
      recursos ni tampoco pasarse, lo que causaría retrasos por cambios de contexto
      entre los hilos asignados a cada core, como pasa cuando los hilos son 24
    - Además, el uso intensivo de 'pow()' en cada iteración implica un alto
      coste computacional; cuando los hilos superan los recursos físicos,
      ese coste no se reduce más.

Conclusión:
    Incrementar el número de hilos por encima de los núcleos físicos o de los
    hilos hardware del procesador no mejora el rendimiento. El tiempo de
    ejecución se estabiliza o incluso aumenta debido al overhead de gestión
    del sistema operativo. Poner un número de hilos inferior a la capacidad
    de hilos físicos no aprovecha los recursos disponibles, y el paralelismo
    conseguido es lento en comparación con números de hilos superiores.
    Por tanto, el número óptimo de hilos es igual o muy muy próximo 
    al número de hilos hardware disponibles.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <sys/time.h>

/*NOTA: ESTRUCTURA DE TIMEVAL
struct timeval {
    time_t      tv_sec;   // Segundos desde 1970
    suseconds_t tv_usec;  // Microsegundos (0 a 999999)
};
*/

#define H 16                //Definimos el número de hilos igual que el número de cores, que se puede ver con el comando "lscpu" en el terminal, que sacaŕa, entre otras, unas líneas así:
                           //Hilo(s) de procesamiento por núcleo: 2   (Esto lo usaremos para el apartado h)
                           //Núcleo(s) por «socket»: 8   Nº de cores
//Para el apartado h, cambiamos esta constante
//Estudiaremos tres casos remarcables: que el número de hilos del programa sea igual al número de cores de nuestro ordenador, igual al número de hilos (entre el número de cores y el número de hilos del ordenador si tiene más de un hilo por core), o mayor al númer de hilos. En este caso, H valdrá 8, 16 y 24
#define N_TERMS 1000000000   //La constante para hacer la suma de Basilea, hacemos mil millones de sumas

typedef unsigned long long ull;    //Para definir los unsigned long long más rápido

typedef struct{      //Definimos una estructura para guardar la información de cada hilo creado por el hilo main de este proceso:
    int id_hilo;         //Su identificador
    ull num_inicio;      //El número de suma en el que empieza a hacer el cálculo
    ull num_final;        //El número de suma en el que acaba de hacer el cálculo
    double suma_parcial;         //Guardamos la suma parcial que ha realizado el hilo
}struct_hilos;       //Nombramos la estructura


void* calculo_hilos(void* arg){        //La función calculo_hilos, que es la que ejecutan los hilos, se pasará como tercer argumento de pthread_create, para indicarle al hilo que se crea la función que tiene que ejecutar, y el argumento a la función del hilo, en este caso arg que será el struct que contendrá el identificador del hilo, las variables que marcan el inicio y el final de su bloque de sumas y la variable para guardar la suma parcial cuando acabe, se pasará como puntero en el último argumento de pthread_create
    struct_hilos* targ = (struct_hilos*)arg;    //Convertimos el void* que pasamos como argumento a pthread_create al struct struct_hilos para usar el contenido
    double suma_local = 0.0;  //Variable local de cada hilo en la que almacenará el resultado después de hacer las sumas que le corresponden

//DOS OPCIONES PARA EL BUCLE DE SUMA: OPCIÓN A: PROBLEMA DE BASILEA; OPCIÓN B: COMPROBACIÓN DE LA REALIZACIÓN DE TODAS LAS OPERACIONES (solo una activa a la vez)

    //OPCIÓN A: PROBLEMA DE BASILEA    
    for (ull i = targ->num_inicio; i <= targ->num_final; ++i){    //Vamos desde la primera hasta la última de las sumas que le corresponden al hilo, de una en una
        double di = (double)i;    //Convertimos i a double para pasárselo como argumento a pow                                                
        double denom = pow(di, 2.0);  //Elevamos n (i) al cuadrado, y ese es el denominador del término de esta suma
        suma_local += 1.0/denom;      //Sumamos el término 1/n² (1/i²) correspondiente a esta suma
    }

    //OPCIÓN B: COMPROBACIÓN DE LA REALIZACIÓN DE TODAS LAS OPERACIONES
/*
    for (ull i = targ->num_inicio; i <= targ->num_final; ++i){  //Vamos desde la primera hasta la última de las sumas que le corresponden al hilo, de una en una{
        suma_local += 1.0;        //Sumamos 1 por suma. Al final, la suma paralela realizada por el hilo principal dará el número total de iteraciones, con lo que comprobaremos si no se ha saltado ninguna                            
    }

*/
    targ->suma_parcial = suma_local;      //Asignamos a la variable del hilo para la suma parcial la variable suma_local que acabamos de calcular con las sumas
    return NULL;      //No le devolvemos nada al proceso que creó al hilo
}

//Definimos un método que nos calcule la diferencia entre las dos medidas de tiempo (con gettimeofday() en este caso) para ver la diferencia entre el cálculo en paralelo con todos los hilos de la suma y la suma que hace el hilo principal secuencialmente solo; o la diferencia entre dos llamadas a gettimeofday()
double timeval_diff_sec(struct timeval *fin_calculo, struct timeval *inicio_calculo){  //Pasamos como argumentos las estructuras con la medida de gettimeofday() al final y al principio del cálculo
    return (double)(fin_calculo->tv_sec - inicio_calculo->tv_sec) + (double)(fin_calculo->tv_usec - inicio_calculo->tv_usec) * 1e-6;    //Restamos los segundos enteros y los micorsegundos, y luego los sumamos para obtenere el tiempo exacto
}

//Calculamos el tiempo que consume una llamada a gettimeofday() recogiendo los tiempos de dos gettimeofday() con ningún código entre ellos
double medir_overhead(){
    struct timeval t1, t2;       //Definimos dos estructuras para recoger los dos tiempos
    gettimeofday(&t1, NULL);     //Llamamos a gettimeofday() y guardamos la hora en t1
    gettimeofday(&t2, NULL);     //Llamamos otra vez a gettimeofday() y guardamos la hora en t2. Lo único que hay entre las dos medidas es una llamada a gettimeofday()
    return timeval_diff_sec(&t2, &t1);   //Usamos la función para restar dos medidas de tiempo que definimos para ello previamente; lo que devuelva será el overhead, el tiempo que consume una llamada a gettimeofday()
}

int main(void){        //La función del hilo principal
    const ull N = (ull) N_TERMS;     //Definimos la variable que contiene el número de sumas como un unsigned long long
    const int num_hilos = H;    //Definimos el número de hilos que vamos a crear para que calculen las sumas paralelas   

    printf("=== Problema de Basilea con hilos ===\n");   //Imprimimos cabecera
    printf("N = %" PRIu64 ", H = %d\n", (uint64_t)N, num_hilos);    //Imprimimos el número de sumas con el formato adecuado para un unsigned int de 64 bits y el número de hilos que vamos a usar, igual al número de cores

    ull sumas_base = N / (ull)num_hilos;      //Calculamos el número de sumas que va a hacer cada hilo
    ull sumas_res  = N % (ull)num_hilos;      //Si la división no es exacta, las sumas restantes que calculamos aquí las hace el primer hilo al cual se las añadiremos

    pthread_t hilos[H];     //Declaramos una conjunto de variables de tipo pthread_t(tipo de datos que representa un hilo) que tendrá tantos elementos como el número de hilos que queremos crear, que definimos en la constante H.
    
    struct_hilos args[H];    //Creamos un conjunto de structs de hilos, tantos como el número de hilos que queremos crear, que definimos en la constante H. Cada elemento será un struct del tipo struct_hilos que contendrá la información que se le asignará a cada hilo

    double overhead = medir_overhead();     //Medimos el overhead de gettimeofday() con la función que definimos previamente para ello

    struct timeval inicio_paralelo, fin_paralelo;     //Definimos dos estructuras para guardar los tiempos de inicio y de fin del cálculo en paralelo de las sumas por parte de los hilos

    int comprobacion;    //Creamos una variable para comprobar que pthread_create y pthread_join ha funcionando correctamente

    gettimeofday(&inicio_paralelo, NULL);     //Medimos el tiempo cuando empezamos los cálculos en paralelo de los hilos

    //Creamos los hilos
    for (int j = 0; j < num_hilos; ++j){       //Tantas iteraciones como el número de hilos que queremos crear para crearlos todos
        ull inicio, fin, cuenta;       //Definimos tres variables: una para marcar el número de suma en el que iniciará cada hilo, otra para marcar el número de suma en el que finalizará cada hilo y otra para marcar el número de sumas totales que hará el hilo
        if (j == 0){     //Si es el primer hilo
            cuenta = sumas_base + sumas_res;     //Las sumas totales del primer hilo son las correspondientes a la división exacta del número de sumas entre el número de hilos, a las que sumamos las sumas que quedan si el número de sumas totales no es divisible por el número de hilos
            inicio = 1ULL;     //El primer hilo comienza con la primera suma
        }else{        //Si estamos en cualquier otro hilo que no sea el primero
            cuenta = sumas_base;        //Las sumas totales del resto de los hilos son las adjudicadas por la división exacta del número de sumas entre el número de hilos
            inicio = sumas_res + sumas_base * (ull)j + 1ULL;   //El resto de los hilos comienzan por donde finalizan todos los hilos anteriores (sumas_base * (ull)j) más el resto de sumas que se le sumó al primer hilo más 1 para iniciar la serie en el 1
							       
	//NOTA: En referencia a lo anterior, por ejemplo, supongamos:
	//N = 10, H = 3
	//sumas_base = n / H = 3
	//sumas_res = n % H = 1
	//| Hilo | Inicio                                              | Count | Fin |
	//| ---- | --------------------------------------------------- | ----- | --- |
	//| 0    | 1 (empezamos en 1 para evitar la división por cero) | 3+1=4 | 4   |
	//| 1    | 1 + 3*1 = 4 → pero sumas_res=1 → 5                  | 3     | 7   |
	//| 2    | 1 + 3*2 = 7 → sumas_res=1 → 8                       | 3     | 10  |

        }
        fin = inicio + cuenta - 1ULL;   //Calculamos el último índice que procesará el hilo:
	     //inicio + cuenta; apunta al número justo después del último que queremos sumar
	     //- 1ULL; retrocede uno para que fin sea el último número real que toca el hilo, porque el primer número ya cuenta como inicio y empezamos en 1

	//NOTA: En referencia a lo anterior, por ejemplo, supongamos:
	//Supongamos: inicio = 5, cuenta = 3
	//Entonces: fin = 5 + 3 - 1 = 7
	//Significa que este hilo hará las sumas 5, 6 y 7

	//Asignamos esta información al struct del hilo
        args[j].id_hilo = j;     //El identificador, el número que indica el orden en el que fue creado 
        args[j].num_inicio = inicio;       //El número de suma en el que empieza a hacer el cálculo, que calculamos previamente
        args[j].num_final = fin;   //EL número de suma en el que acaba de hacer el cálculo, que calculamos previamente
        args[j].suma_parcial = 0.0;   //La variable suma_parcial que vale 0.0, para que comience a sumar

	printf("  [Hilo %d] empieza=%" PRIu64 ", acaba=%" PRIu64 " (sumas=%" PRIu64 ")\n",j, (uint64_t)inicio, (uint64_t)fin, (uint64_t)cuenta);      //Imprimimos, para cada hilo, el número de suma en el que empieza, en el que acaba y el número total de sumas que hace
	
	comprobacion = pthread_create(&hilos[j], NULL, calculo_hilos, &args[j]);   //Pasamos el identificador del hilo j; lo creamos con los atributos por defecto; pasamos la función que tiene que ejecutar, calculo_hilos; y le pasamos como argumento de la función args[j], el struct que contiene toda la información para que realice su fragmento de cálculo, que acabamos de asignar antes de llamar a pthread_create(). Guardamos la salida en la variable que creamos para ello, y comprobaremos que es igual a 0 si el hilo se ha creado correctamente
										 
        if(comprobacion != 0){     //Si pthread_create() devuelve algún valor que no sea 0, significa que hubo un error creando el hilo
                printf("Error creando el hilo %d\n",j+1);   //Informamos si se ha producido un error al usuario
        }
    }

    for (int j = 0; j < num_hilos; ++j){    //Tantas iteraciones como el número de hilos que queremos recoger para esperarlos a todos
    
    	comprobacion = pthread_join(hilos[j], NULL);     //El hilo principal espera al hilo correspondiente si todavía no ha acabado o lo recoge directamente; en los dos casos, este hilo principal que llama a pthread_join se queda bloqueado hasta que el hilo que quiere recoger ha acabado, y lo puede recoger y liberar todos sus datos. Guardamos la salida en la variable que creamos para ello, y comprobaremos que es igual a 0, si el hilo se ha creado correctamente
	//El segundo argumento es NULL porque no nos interesa obtener el valor que devuelve el hilo
        if(comprobacion != 0){   //Si pthread_join() devuelve algún valor que no sea 0, significa que hubo un error recogiendo el hilo
                printf("Error recogiendo el hilo %d\n", j+1);  //Informamos al usuario si se ha producido ese error
        }
    }

    gettimeofday(&fin_paralelo, NULL);    //Una vez que todos los hilos han acabado y los hemos recogido, medimos la hora de nuevo
    double tiempo_paralelo = timeval_diff_sec(&fin_paralelo, &inicio_paralelo) - overhead;   //Calculamos el tiempo que han tardado en hacer las sumas los hilos en paralelo restando la hora a la que acabaron y a la que iniciaron con la función que definimos para restar tiempos previamente, y a su vez le restamos el overhead, para que no distorsione los tiempos la llamada a gettimeofday()

    //El hilo principal suma las sumas parciales de todos los hilos
    double suma_paralela = 0.0;    //Definimos una variable donde almacenar la suma total de las sumas parciales de los hijos
    for(int j = 0; j < num_hilos; ++j)     //Recorremos todos los hilos
        suma_paralela += args[j].suma_parcial;    //Accedemos a su struct asociado del tipo struct hilos, al campo que guarda la suma parcial que han calculado, y la sumamos a la variable suma_paralela para obtener la suma total de todas las sumas de todos los hilos

    //El hilo principal hace el cálculo secuencial de todas las sumas para comparar tiempos
    struct timeval inicio_secuencial, fin_secuencial;   //Definimos dos estructuras para guardar los tiempos de inicio y de fin del cálculo secuencial de todas las sumas por el hilo principal

    double suma_secuencial = 0.0;   //Definimos una variable para guardar el resultado que obtenga el hilo principal al hacer las sumas secuencialmente

    gettimeofday(&inicio_secuencial, NULL);    //Medimos el tiempo cuando empezamos los cálculos en paralelo de los hilos

    //DOS OPCIONES PARA EL BUCLE DE SUMA: OPCIÓN A: PROBLEMA DE BASILEA; OPCIÓN B: COMPROBACIÓN DE LA REALIZACIÓN DE TODAS LAS OPERACIONES (solo una activa a la vez)

    //OPCIÓN A: PROBLEMA DE BASILEA 
    for(ull i = 1ULL; i <= N; ++i){        //Iteramos desde la suma 1 hasta la N-1; siendo N = 1000000000, mil millones de sumas
        double di = (double)i;        //Convertimos i a double para pasárselo como argumento a pow
        double denom = pow(di, 2.0);  //Elevamos n (i) al cuadrado, y ese es el denominador del término de esta suma
        suma_secuencial += 1.0/denom;      //Sumamos el término 1/n² (1/i²) correspondiente a esta suma
    }
    
    //OPCIÓN B: COMPROBACIÓN DE LA REALIZACIÓN DE TODAS LAS OPERACIONES
/*
    for(ull i = 1ULL; i <= N; ++i){        //Iteramos desde la suma 1 hasta la N-1; siendo N = 1000000000, mil millones de sumas
        suma_secuencial += 1.0;      //Sumamos 1 correspondiente a esta suma
    }
*/
    gettimeofday(&fin_secuencial, NULL);     //Una vez que el hilo principal ha realizado todas las sumas, medimos la hora de nuevo
    double tiempo_secuencial = timeval_diff_sec(&fin_secuencial, &inicio_secuencial) - overhead;    //Calculamos el tiempo que ha tardado en hacer las sumas el hilo principal restando la hora a la que acabó y a la que inició con la función que definimos para restar tiempos previamente, y a su vez le restamos el overhead, para que no distorsione los tiempos la llamada a gettimeofday()

    //Imprimimos los resultados obtenidos
    double num_exacto = (M_PI * M_PI) / 6.0;      //M_PI es una constante de la librería math.h que representa el número PI
    printf("\nRESULTADOS:\n");
    //%.15g muestra 15 cifras significativas
    printf("  Suma paralela   = %.15g\n", suma_paralela);       //Imprimimos el resultado obtenido al hacer las sumas repartidas por los hilos paralelamente
    printf("  Suma secuencial = %.15g\n", suma_secuencial);        //Imprimimos el resultado obtenido al haber hecho todas las sumas el hilo principal
    printf("  π²/6 (exacto)   = %.15g\n", num_exacto);          //Imprimimos el número exacto de referencia para el resultado

    printf("\nDiferencias:\n");
    //fabs() devuelve el valor absoluto de un double
    printf("  |Suma paralela - Suma secuencial|   = %.15g\n", fabs(suma_paralela - suma_secuencial));    //Imprimimos la diferencia del resultado obtenido entre las sumas de los hilos por separado y las sumas realizadas por el hilo principal
    printf("  |Suma paralela - Número exacto| = %.15g\n", fabs(suma_paralela - num_exacto));     //Imprimimos la diferencia del resultado obtenido entre las sumas de los hilos por separado y el número exacto de referencia
    printf("  |Suma secuencial - Número exacto| = %.15g\n", fabs(suma_secuencial - num_exacto));    //Imprimimos la diferencia del resultado obtenido entre las sumas realizadas por el hilo principal y el número exacto de referencia

    printf("\nTiempos:\n");
    //.6f imprime 6 cifras significativas
    printf("  Paralelo   = %.6f s\n", tiempo_paralelo);           //Imprimimos el tiempo que tardan los hilos por separado en calcular el resultado
    printf("  Secuencial = %.6f s\n", tiempo_secuencial);      //Imprimimos el tiempo que tardó el hilo principal en calcular el resultado
    printf("  Ahorro de tiempo realizando el cálculo con hilos respecto al secuencial con un solo hilo   = %.3f\n", tiempo_secuencial / tiempo_paralelo);       //Imprimimos cuánto tiempo se ahorra calculando el resultado paralelamente con varios hilos respecto a calcularlo secuencialmente con uno solo

    return EXIT_SUCCESS;
}

/*
SALIDA 1: Problema de Basilea con hilos (versión para verificar iteraciones)
=== Problema de Basilea con hilos ===
N = 1000000000, H = 8 (núcleos físicos)
  [Hilo 0] empieza=1, acaba=125000000 (sumas=125000000)
  [Hilo 1] empieza=125000001, acaba=250000000 (sumas=125000000)
  [Hilo 2] empieza=250000001, acaba=375000000 (sumas=125000000)
  [Hilo 3] empieza=375000001, acaba=500000000 (sumas=125000000)
  [Hilo 4] empieza=500000001, acaba=625000000 (sumas=125000000)
  [Hilo 5] empieza=625000001, acaba=750000000 (sumas=125000000)
  [Hilo 6] empieza=750000001, acaba=875000000 (sumas=125000000)
  [Hilo 7] empieza=875000001, acaba=1000000000 (sumas=125000000)

RESULTADOS:
  Suma paralela   = 1000000000
  Suma secuencial = 1000000000
  π²/6 (exacto)   = 1.64493406684823

Diferencias:
  |Suma paralela - Suma secuencial|   = 0
  |Suma paralela - Número exacto| = 999999998.355066
  |Suma secuencial - Número exacto| = 999999998.355066

Tiempos:
  Paralelo   = 0.359548 s
  Secuencial = 2.550239 s
  Ahorro de tiempo realizando el cálculo con hilos respecto al secuencial con un solo hilo   = 7.093

Suma paralela = 1000000000 y Suma secuencial = 1000000000; es correcto, comprobamos que el número de sumas que hacemos es 1000000000 si sumamos 1 en cada iteración, tanto de los hilos paralelos como del hilo principal

Los rangos por hilo impresos muestran una distribución bloques correcta: cada hilo hace 125000000 iteraciones (8 hilos; 8×125000000 = 1e9)

SALIDA 2: Problema de Basilea con hilos (versión nº hilos = nº cores)
=== Problema de Basilea con hilos ===
N = 1000000000, H = 8 (núcleos físicos)
  [Hilo 0] empieza=1, acaba=125000000 (sumas=125000000)
  [Hilo 1] empieza=125000001, acaba=250000000 (sumas=125000000)
  [Hilo 2] empieza=250000001, acaba=375000000 (sumas=125000000)
  [Hilo 3] empieza=375000001, acaba=500000000 (sumas=125000000)
  [Hilo 4] empieza=500000001, acaba=625000000 (sumas=125000000)
  [Hilo 5] empieza=625000001, acaba=750000000 (sumas=125000000)
  [Hilo 6] empieza=750000001, acaba=875000000 (sumas=125000000)
  [Hilo 7] empieza=875000001, acaba=1000000000 (sumas=125000000)

RESULTADOS:
  Suma paralela   = 1.64493406483457
  Suma secuencial = 1.64493405783458
  π²/6 (exacto)   = 1.64493406684823

Diferencias:
  |Suma paralela - Suma secuencial|   = 6.99999969100418e-09
  |Suma paralela - Número exacto| = 2.01365168983614e-09
  |Suma secuencial - Número exacto| = 9.01365138084032e-09

Tiempos:
  Paralelo   = 2.556955 s
  Secuencial = 15.606729 s
  Ahorro de tiempo realizando el cálculo con hilos respecto al secuencial con un solo hilo   = 6.104×

Suma paralela = 1.64493406483457, Suma secuencial = 1.64493405783458 y el número exacto es 1.64493406684823; diferencias del orden 1e-9 ínfimas y razonables

Tiempos: paralelo ≈ 2.56 s vs secuencial ≈ 15.61 s; el cálculo en paralelo es 6.1 veces más rápido. Es un ahorro de tiempo razonable (no exactamente 8 por el tiempo consumido en la creación de hilos, coste de pow, memoria...)

SALIDA 3: Problema de Basilea con hilos (versión nº hilos = nº hilos físicos (2*core)
=== Problema de Basilea con hilos ===
N = 1000000000, H = 16 (hilos físicos)
  [Hilo 0] empieza=1, acaba=62500000 (sumas=62500000)
  [Hilo 1] empieza=62500001, acaba=125000000 (sumas=62500000)
  [Hilo 2] empieza=125000001, acaba=187500000 (sumas=62500000)
  [Hilo 3] empieza=187500001, acaba=250000000 (sumas=62500000)
  [Hilo 4] empieza=250000001, acaba=312500000 (sumas=62500000)
  [Hilo 5] empieza=312500001, acaba=375000000 (sumas=62500000)
  [Hilo 6] empieza=375000001, acaba=437500000 (sumas=62500000)
  [Hilo 7] empieza=437500001, acaba=500000000 (sumas=62500000)
  [Hilo 8] empieza=500000001, acaba=562500000 (sumas=62500000)
  [Hilo 9] empieza=562500001, acaba=625000000 (sumas=62500000)
  [Hilo 10] empieza=625000001, acaba=687500000 (sumas=62500000)
  [Hilo 11] empieza=687500001, acaba=750000000 (sumas=62500000)
  [Hilo 12] empieza=750000001, acaba=812500000 (sumas=62500000)
  [Hilo 13] empieza=812500001, acaba=875000000 (sumas=62500000)
  [Hilo 14] empieza=875000001, acaba=937500000 (sumas=62500000)
  [Hilo 15] empieza=937500001, acaba=1000000000 (sumas=62500000)

RESULTADOS:
  Suma paralela   = 1.64493406563894
  Suma secuencial = 1.64493405783458
  π²/6 (exacto)   = 1.64493406684823

Diferencias:
  |Suma paralela - Suma secuencial|   = 7.80436382186167e-09
  |Suma paralela - Número exacto| = 1.20928755897864e-09
  |Suma secuencial - Número exacto| = 9.01365138084032e-09

Tiempos:
  Paralelo   = 1.977594 s
  Secuencial = 15.603201 s
  Ahorro de tiempo realizando el cálculo con hilos respecto al secuencial con un solo hilo = 7.890

Las diferencias de resultados entre la suma paralela y la suma secuencial con respecto al número exacto son igualmente pequeñas (1e-9).

Tiempo paralelo ≈ 1.98 s, que es mejor que con 8 hilos (al admitir 16 hilos físicos, esto tiene sentido). Ahorro de tiempo realizando el cálculo con hilos respecto al secuencial con un solo hilo = 7.89, mayor que con nº hilos = nº cores al aprovechar todos los recursos de los que se disponen

SALIDA 4: Problema de Basilea con hilos (versión nº hilos > nº cores y nº hilos > nº hilos físicos))
=== Problema de Basilea con hilos ===
N = 1000000000, H = 24 (más hilos que cores e hilos físicos)
  [Hilo 0] empieza=1, acaba=41666682 (sumas=41666682)
  [Hilo 1] empieza=41666683, acaba=83333348 (sumas=41666666)
  [Hilo 2] empieza=83333349, acaba=125000014 (sumas=41666666)
  [Hilo 3] empieza=125000015, acaba=166666680 (sumas=41666666)
  [Hilo 4] empieza=166666681, acaba=208333346 (sumas=41666666)
  [Hilo 5] empieza=208333347, acaba=250000012 (sumas=41666666)
  [Hilo 6] empieza=250000013, acaba=291666678 (sumas=41666666)
  [Hilo 7] empieza=291666679, acaba=333333344 (sumas=41666666)
  [Hilo 8] empieza=333333345, acaba=375000010 (sumas=41666666)
  [Hilo 9] empieza=375000011, acaba=416666676 (sumas=41666666)
  [Hilo 10] empieza=416666677, acaba=458333342 (sumas=41666666)
  [Hilo 11] empieza=458333343, acaba=500000008 (sumas=41666666)
  [Hilo 12] empieza=500000009, acaba=541666674 (sumas=41666666)
  [Hilo 13] empieza=541666675, acaba=583333340 (sumas=41666666)
  [Hilo 14] empieza=583333341, acaba=625000006 (sumas=41666666)
  [Hilo 15] empieza=625000007, acaba=666666672 (sumas=41666666)
  [Hilo 16] empieza=666666673, acaba=708333338 (sumas=41666666)
  [Hilo 17] empieza=708333339, acaba=750000004 (sumas=41666666)
  [Hilo 18] empieza=750000005, acaba=791666670 (sumas=41666666)
  [Hilo 19] empieza=791666671, acaba=833333336 (sumas=41666666)
  [Hilo 20] empieza=833333337, acaba=875000002 (sumas=41666666)
  [Hilo 21] empieza=875000003, acaba=916666668 (sumas=41666666)
  [Hilo 22] empieza=916666669, acaba=958333334 (sumas=41666666)
  [Hilo 23] empieza=958333335, acaba=1000000000 (sumas=41666666)

RESULTADOS:
  Suma paralela   = 1.64493406592566
  Suma secuencial = 1.64493405783458
  π²/6 (exacto)   = 1.64493406684823

Diferencias:
  |Suma paralela - Suma secuencial|   = 8.09108313681861e-09
  |Suma paralela - Número exacto| = 9.22568244021704e-10
  |Suma secuencial - Número exacto| = 9.01365138084032e-09

Tiempos:
  Paralelo   = 2.074348 s
  Secuencial = 15.793240 s
  Ahorro de tiempo realizando el cálculo con hilos respecto al secuencial con un solo hilo   = 7.614
*/  
  
   
