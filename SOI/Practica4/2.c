#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

//Variable global compartida (como un archivo compartido, todos acceden, modifican y los cambios que haga uno afectan al instante al resto
int contador_global = 0;

//Función que ejecutará cada hilo
void* tarea(void* arg){   //La función tarea, que es la que ejecutan los hilos, se pasará como tercer argumento de pthread_create, para indicarle al hilo que se crea la función que tiene que ejecutar, y el argumento a la función del hilo, en este caso arg que será el nombre del hilo, se pasará como puntero en el último argumento de pthread_create
    int id = *(int*)arg;      //Convertimos el void* que pasamos como argumento a pthread_create a int para usar el id como identificador de cada hilo
	
    if(id == 1){    //Hacemos que el primer hilo imprima un ps para poder ver por el terminal que se han creado todos los hilos, que tienen el mismo PID y solo cambian en el campo LWP, el correspondiente al ID de cada hilo. Además, el campo NLWP, que cuenta el número de hilos que tiene cada proceso, también es el mismo (todos los hilos salen de un proceso)
		     //VER TABLA CORRESPONDIENTE A LA SALIDA 1 EN EL COMENTARIO DEL FINAL
	    printf("PS1\n");
    }

    sleep(10);    //Ponemos todos los hilos a dormir para que nos de tiempo a hacer el ps

    printf("[Hilo-%d] Inicia: contador_global=%d\n", id, contador_global);  //Para mantener a los hilos 1 y 2 ocupados, hacemos que modifiquen el contador global; antes de eso imprimimos el valor del contador local antes de que empiecen

    //Queremos que el tercer hilo haga un fork
    if(id == 3){    //Nos "situamos" en el tercer hilo

        pid_t pid = fork();    //El tercer hijo hace el fork 
        if(pid < 0){           //Si la variable pid es menor que 0, hubo un error al crear el proceso hijo
            printf("Error al crear el proceso hijo\n");  //Informamos de que hubo un error
        }else if(pid == 0){    //Si la variable pid vale 0, estamos en el proceso hijo
            //PROCESO HIJO
            printf("[Hijo] PID=%d, este es el hijo del hilo-3\n", getpid());  //Imprimimos un mensaje del hijo identificándose
	    printf("PS2\n");   //Hacemos que el hijo imprima un ps para poder ver en el terminal que se ha creado, que es hijo del proceso principal y que aunque a efectos el padre es el proceso principal en su conjunto, solo replica el hilo que lo ha creado: solo tiene un hilo.
			       //VER TABLA CORRESPONDIENTE A LA SALIDA 2 EN EL COMENTARIO DEL FINAL
            sleep(15); //Ponemos el proceso hijo a dormir para que nos de tiempo a inspeccionarlo
            exit(0);  //Fin del proceso hijo
        }else{     //Si la variable pid es natural, entonces estamos en el proceso padre (a efectos de creación es el hilo 3, aunque el padre "mayor" sea el proceso en su conjunto
            //PROCESO PADRE
            printf("[Hilo-3] Hijo creado con PID=%d\n", pid);   //El hilo 3 confirma que ha creado al proceso hijo
            sleep(10);    //Intentamos sincronizar el fin de los tres hilos  
        }		
    }

    //Iteraciones para mantener los hilos 1 y 2 activos para que al hilo 3 le de tiempo a crear el proceso hijo antes de que los hilos 1 y 2 acaben
    if(id == 1 || id == 2){   //Nos "situamos" en los hijos 1 y 2
	for(int i = 0; i < 5; i++){       //Cada uno incrementa el contador global 5 veces y va imprimiendo las modificaciones (se ven las modificaciones de uno que afectan al otro)
        	contador_global++;
        	printf("[Hilo-%d] Iteración %d: contador_global=%d\n", id, i + 1, contador_global);
        sleep(1);
    	}
    }

    printf("[Hilo-%d] Termina: contador_global=%d\n", id, contador_global);    //Cuando los hilos terminan, informan e impriman la variable global
    return NULL;    //No le devolvemos nada al proceso que creó al hilo
}

int main(){    //Función principal

    pthread_t hilos[3];    //Declaramos una conjunto de variables de tipo pthread_t(tipo de datos que representa un hilo) que tendrá tres elementos, cada uno de los cuales guarda el ID de un hilo que después se creará con pthread_create 
    int ids[3] = {1, 2, 3};   //Creamos un conjunto de números que le asignaremos respectivamente a cada hilo para identificarlos

    int comprobacion;    //Creamos una variable para comprobar que pthread_create y pthread_join ha funcionando correctamente

    //Creamos los hilos
    for(int i = 0; i < 3; i++){    //3 iteraciones para crear tres hilos
        comprobacion = pthread_create(&hilos[i], NULL, tarea, &ids[i]);   //Pasamos el identificador del hilo i; lo creamos con los atributo por defecto; pasamos la función que tiene que ejecutar, tarea; y le pasamos como argumento de la función ids[i], el identificador que le queremos poner al hilo y que le asignamos a través de la función tarea, que nos lo pide como argumento. Guardamos la salida en la variable que creamos para ello, y comprobaremos que es igual a 0 si el hilo se ha creado correctamente
	if(comprobacion != 0){     //Si pthread_create() devuelve algún valor que no sea 0, significa que hubo un error creando el hilo
		printf("Error creando el hilo %d\n",i+1);   //Informamos si se ha producido un error al usuario
	}
    }

    //Esperamos a que terminen todos los hilos
    for(int i = 0; i < 3; i++){    //Iteramos tres veces para recoger todos los hilos
        comprobacion = pthread_join(hilos[i], NULL);     //El hilo principal espera al hilo correspondiente si todavía no ha acabado o lo recoge directamente; en los dos casos, este hilo principal que llama a pthread_join se queda bloqueado hasta que el hilo que quiere recoger ha acabado, y lo puede recoger y liberar todos sus datos. Guardamos la salida en la variable que creamos para ello, y comprobaremos que es igual a 0, si el hilo se ha creado correctamente

	if(comprobacion != 0){   //Si pthread_join() devuelve algún valor que no sea 0, significa que hubo un error recogiendo el hilo
		printf("Error recogiendo el hilo %d\n", i+1);  //Informamos al usuario si se ha producido ese error
	}
    }

    printf("PS3\n");    //Hacemos que el hilo principal imprima un ps para poder ver en el terminal que ya se han recogido todos los hilos, que han terminado, y que sigue existiendo el proceso hijo, al ser el "padre mayor" el proceso en su conjunto, a pesar de que su "padre creador", el hilo 3, ya ha terminado. En este momento ya solo quedan activos el hilo principal del proceso padre (el que creó los otros tres hilos), y el proceso hijo que creó el hilo 3
                               //VER TABLA CORRESPONDIENTE A LA SALIDA 3 EN EL COMENTARIO DEL FINAL

    printf("\n[MAIN] Valor final de contador_global = %d\n", contador_global);   //Imprimimos el valor final del contador global después de que los hilos 1 y 2 estuviesen aumentándolo
    sleep(10);   //Ponemos a dormir el hilo principal para que acabe el proceso hijo creado por el hilo 3

    printf("PS4\n");     //Hacemos que el hilo principal imprima un ps para poder ver que, una vez que el proceso hijo creado por el hilo 3 ya ha terminado, se convierte en zombi: esto nos confirma que realmente, a pesar de que lo ha creado el hilo 3, el padre "importante", el "absoluto"  es el proceso en su conjunto, por eso el sistema operativo, el shell, mantiene al proceso hijo como zombi, esperando hasta que finalice el hilo principal (el último de los hilos del proceso padre que queda activo) y se ponga fin al proceso padre.
    sleep(5);      //Ponemos el hilo principal en pausa para hacer el ps

    return 0;    //Acabamos el programa
}


/* Salidas de ps y tablas

## Salida 1
rosa_gonzalez6482@HPRosa:~/SOI/P4$ ps -eLf | grep ej2
rosa_go+   16707   14532   16707  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16707   14532   16708  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16707   14532   16709  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16707   14532   16710  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16714   14556   16714  0    1 11:25 pts/20   00:00:00 grep --color=auto ej2

### Tabla 1: Fase 1 (todos los hilos creados, antes del fork)

| PID   | PPID  | LWP   | NLWP | TTY    | Comando |       
|-------|-------|-------|------|--------|---------|
| 16707 | 14532 | 16707 | 4    | pts/19 | ./ej2   |       //Podemos ver que tenemos el mismo PID 4 veces, con el mismo PPID (proceso padre); pero los LWP son diferentes: esto significa
| 16707 | 14532 | 16708 | 4    | pts/19 | ./ej2   |       //que tenemos un mismo proceso con 4 hilos
| 16707 | 14532 | 16709 | 4    | pts/19 | ./ej2   |       //Esto nos lo confirma la columna NLWP, que recoge el número de hilos del proceso al que se hace referencia; en los 4 hilos
| 16707 | 14532 | 16710 | 4    | pts/19 | ./ej2   |       //la cuenta de hilos del proceso al que pertence es 4: el hilo principal y los tres hilos que se han creado

//NOTA: El número LWP del hilo principal y el PID del proceso al que pretenece dicho hilo coinciden
---

## Salida 2
rosa_gonzalez6482@HPRosa:~/SOI/P4$ ps -eLf | grep ej2
rosa_go+   16707   14532   16707  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16707   14532   16708  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16707   14532   16709  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16707   14532   16710  0    4 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16716   16707   16716  0    1 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16718   14556   16718  0    1 11:25 pts/20   00:00:00 grep --color=auto ej2

### Tabla 2: Fase 2 (después de fork, hijo activo)

| PID   | PPID  | LWP   | NLWP | TTY    | Comando |
|-------|-------|-------|------|--------|---------|
| 16707 | 14532 | 16707 | 4    | pts/19 | ./ej2   |    //Vemos lo mismo que antes, los 4 hilos del proceso padre y, además, ahora nos aparece un nuevo proceso con un PID diferente
| 16707 | 14532 | 16708 | 4    | pts/19 | ./ej2   |    //(el proceso que ha creado el hilo 3), que solo tiene un hilo, el que ha "copiado" del hilo que lo ha creado
| 16707 | 14532 | 16709 | 4    | pts/19 | ./ej2   |    //Que solo tiene un hilo lo confirmamos al ver su columna NLWP
| 16707 | 14532 | 16710 | 4    | pts/19 | ./ej2   |    //Sin embargo, como podemos ver en su campo PPID, el padre de este proceso es el proceso al que pertenece ese hilo en su conjunto, 
| 16716 | 16707 | 16716 | 1    | pts/19 | ./ej2   |    //no el hilo en sí, que solo ejerce de "creador".

---

## Salida 3
rosa_gonzalez6482@HPRosa:~/SOI/P4$ ps -eLf | grep ej2
rosa_go+   16707   14532   16707  0    1 11:25 pts/19   00:00:00 ./ej2                 
rosa_go+   16716   16707   16716  0    1 11:25 pts/19   00:00:00 ./ej2
rosa_go+   16722   14556   16722  0    1 11:25 pts/20   00:00:00 grep --color=auto ej2

### Tabla 3: Fase 3 (H3 terminó, hijo aún activo)

| PID   | PPID  | LWP   | NLWP | TTY    | Comando |
|-------|-------|-------|------|--------|---------|
| 16707 | 14532 | 16707 | 1    | pts/19 | ./ej2   |       //Vemos que, ahora que los hilos creados 1,2 y 3 han acabado, el proceso principal pasa a tener NLWP = 1
| 16716 | 16707 | 16716 | 1    | pts/19 | ./ej2   |       //El proceso hijo todavía sigue activo

---

## Salida 4
rosa_gonzalez6482@HPRosa:~/SOI/P4$ ps -eLf | grep ej2
rosa_go+   16707   14532   16707  0    1 11:25 pts/19   00:00:00 ./ej2                 
rosa_go+   16716   16707   16716  0    1 11:25 pts/19   00:00:00 [ej2] <defunct>         
rosa_go+   16725   14556   16725  0    1 11:26 pts/20   00:00:00 grep --color=auto ej2       

### Tabla 4: Fase 4 (padre terminó hilos, hijo convertido en zombi)

| PID   | PPID  | LWP   | NLWP | TTY    | Comando         |
|-------|-------|-------|------|--------|-----------------|
| 16707 | 14532 | 16707 | 1    | pts/19 | ./ej2           |              //Seguimos teniendo activo el hilo principal del proceso padre
| 16716 | 16707 | 16716 | 1    | pts/19 | [ej2] <defunct> |        //Vemos que al terminar el proceso hijo, se convierte en zombi, lo que confirma que está
									//esperando a que terminen todos los hilos de su padre, que es el proceso en su conjunto								
	
*/
