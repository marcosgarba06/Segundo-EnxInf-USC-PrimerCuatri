#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  // Para sleep

//Variable global compartida, se modificará por todos los hilos, ya que al crearlos, se comparte con todos los hilos los datos generales del proceso
int contador_global = 0;

//Función que ejecutará cada hilo
void* tarea(void* arg) {     //La función tarea, que es la que ejecutan los hilos, se pasará como tercer argumento de pthread_create, para indicarle al hilo que se crea la función que tiene que ejecutar, y el argumento a la función del hilo, en este caso arg que será el nombre del hilo, se pasará como puntero en el último argumento de pthread_create
    char* nombre = (char*) arg;   //Convertimos el void* que pasamos como argumento a pthread_create a char* para usar el nombre solo con fines de impresión
    int local = 0;  //Variable local: propia de cada hilo, aunque se llamen todas local, es como si tuviésemos local1, local2, local3... todas propias de cada hilo 

    printf("[%s] Inicia: contador_global=%d, local=%d\n", nombre, contador_global, local);       //Imprimimos los datos que tiene el hilo antes de iterar

    for (int i = 0; i < 3; i++) {          //Iteramos 3 veces
        contador_global++;  //Modificamos la variable global, que afectará a la variable global en todos los hilos, ya que es como si los datos del proceso que creó todos los hilos fuesen como un archivo compartido al que todos pueden acceder
        local++;            //Modificamos la variable local, única de cada hilo, porque solo la crea él
        printf("[%s] Iteración %d: contador_global=%d, local=%d\n", 
               nombre, i + 1, contador_global, local);           //Imprimimos los datos que tiene el hilo después de cada iteración
        usleep(100000);  // 0.1 segundos                 //Damos un poco de tiempo entre hilos
    }

    printf("[%s] Termina: contador_global=%d, local=%d\n", nombre, contador_global, local);   //Imprimimos los valores de los tres hilos después de iterar, serán iguales, porque la variable global es compartida y porque al hacer el mismo aumento cada hilo su variable local va a valer lo mismo aunque son variables distintas porque cada uno tiene la suya aunque se llamen igual
    return NULL;    //No le devolvemos nada al proceso que creó al hilo
}

int main() {      //Función principal
    pthread_t hilos[3];    //Declaramos una conjunto de variables de tipo pthread_t(tipo de datos que representa un hilo) que tendrá tres elementos, cada uno de los cuales guarda el ID de un hilo que después se creará con pthread_create 
    char* nombres[] = {"Hilo-1", "Hilo-2", "Hilo-3"};   //Creamos un conjunto de nombres que le asignaremos respectivamente a cada hilo

    //Creamos hilos
    for (int i = 0; i < 3; i++) {   //3 iteraciones para crear tres hilos
        pthread_create(&hilos[i], NULL, tarea, nombres[i]);   //Pasamos el identificador del hilo i, lo creamos con los atributod por defecto, pasamos la función que tiene que ejecutar, tarea, y le pasamos como argumento de la función nombres[i], el nombre que le queremos poner al hilo y que le asignamos a través de la función tarea, que nos lo pide como argumento
    }

    // Esperar a que terminen todos los hilos
    for (int i = 0; i < 3; i++) {      //Iteramos tres veces para recoger todos los hilos
        pthread_join(hilos[i], NULL);    //El hilo principal espera al hilo correspondiente si todavía no ha acabado o lo recoge directamente; en los dos casos, este hilo principal que llama a pthread_join se queda bloqueado hasta que el hilo que quiere recoger ha acabado y lo puede recoger y liberar todos sus datos
    }

    printf("\n[MAIN] Valor final de contador_global = %d\n", contador_global);  //Imprimimos el valor final del contador global, después de que todos los hilos lo incrementasen
    return 0;      //Salimos del programa
}

