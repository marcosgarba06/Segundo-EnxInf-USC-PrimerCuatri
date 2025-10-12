#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <math.h>

#define SALIDA_ARCHIVO "resultados.txt"
// Codigo realizado por Marcos García Blanco y Nicolás S. Gómez Forján

// Calcular de dos maneras la media en doble precisión de las tangentes de las raíces cuadradas de los primeros N números naturales
void imprimirArchivo(const char *arquivo, int pid, double val, double tExec)

{
    FILE *f = fopen(arquivo, "a");
    if (f == NULL) {
        printf("Erro abrindo o arquivo %s\n", arquivo);
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%d %.10f %.10f\n", pid, val, tExec);
    fclose(f);
}

void imprimirMedia(const char *arquivo, double val, int media)
{
    FILE *f = fopen(arquivo, "a");
    if (f == NULL) {
        printf("Erro abrindo o arquivo %s\n", arquivo);
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%.10f\n", val/media);
    fclose(f);
}

// TODO: Para Marcos
double calcularTangente(int rangInf, int rangSup)
{
    double suma = 0.0;

    for (int i = rangInf; i < rangSup + 1; i++){
        suma += tan(sqrt(i)); //i esta en radians
    }
    return suma;
}

void calcularSuma(const char *arquivo, int P, double *sumaValores, double *sumaTempos)
{
    FILE *f = fopen(arquivo, "r");
    if (f == NULL) {
        printf("Erro abrindo o arquivo %s\n", arquivo);
        exit(EXIT_FAILURE);
    }
    //inicializar variables
    double val, tempo;
    int pid;
    int flag = 0;
    *sumaValores = 0.0;
    *sumaTempos = 0.0;

    while (fscanf(f, "%d %lf %lf", &pid, &val, &tempo) == 3)
    {
        if (flag < P) { // Solo sumar los P primeros hijos
            *sumaValores += val;
            *sumaTempos += tempo;
        }
        flag ++;
    }
    fclose(f);
}

void diferenciaValores(const char *arquivo, int P, int pidSuma)
{
    // Abrir el archivo
    FILE *f = fopen(arquivo, "r");
    if (f == NULL) {
        printf("Erro abrindo o arquivo %s\n", arquivo);
        exit(EXIT_FAILURE);
    }

    int pid[2];
    double val[2], tempo[2];
    double tTotal;

    // Cojer e imprimir los valores de tiempo de los PID de cálculo
    for (int i=0; i < P; i++)
    {
        if(fscanf(f, "%d %lf %lf", &pid[0], &val[0], &tempo[0]) == 3)
        {
            tTotal += tempo[0];
            printf("O PID %d tardou %fms\n", pid[0], tempo[0]);
        }
        else
        {
            // Si por culalquier motivo se escribiese en el archivo de menos, salir
            printf("EOF chegado prematuramente\n");
            exit(EXIT_FAILURE);
        }
    }

    // Cojer los valores del hijo que calculó la suma y la del cálculo sin hijos
    if(fscanf(f, "%d %lf %lf", &pid[0], &val[0], &tempo[0]) == 3 && fscanf(f, "%d %lf %lf", &pid[1], &val[1], &tempo[1]) == 3)
    {
        // Calcular la diferencia, identificando el proceso de suma con su PID desde los argumentos
        printf("Diferenza de resultados: %f\nDiferenza de tempos: %f\n", labs(val[0] - val[1]), labs(tTotal + (pidSuma == pid[0] ? tempo[1] : tempo[0])  - (pidSuma == pid[0] ? tempo[0] : tempo[1])));
    }
    
}

int main(int argc, char const *argv[])
{

    struct timeval inicio, fin;

    if (argc != 3) {
        printf("Número de argumentos incorrecto. Uso: ./entregable3 <N> <P>\n");
        return 1;
    }

    int N = atoi(argv[1]); //Cantidad de numeros naturales
    int P = atoi(argv[2]); //Cantiadad de procesos hijo
    //Cada hijo se encargara de calcular la media de N/P numeros
    if (N <= 0 || P <= 0 || N < P) {
        printf("Os argumentos deben ser enteiros positivos e N debe ser maior ou igual que P.\n");
        return 1;
    }

    // Calcular o overhead de gettimeofday()
    gettimeofday(&inicio, NULL);
    gettimeofday(&fin, NULL);
    double overhead = (fin.tv_sec - inicio.tv_sec)*1e6 + fin.tv_usec - inicio.tv_usec;

    pid_t proceso[P];

    double resultado = 123;

    double tiempo;

    // Iterar sobre los posibles resultados, dejando al lado el módulo para computarlo posteriormente
    // y no molestar en el proceso del for (ver linea 52)
    for (int i = 0; i < P; i++) {
        // Crear el hijo y entrar en su rama
        // El proceso padre solo creará más hijos
        proceso[i] = fork();
        if (proceso[i] == 0){
            // Coger el tiempo de inicio
            gettimeofday(&inicio, NULL);
            // Calcular la raiz cuadrada de la tangente
            resultado = calcularTangente(i*(N/P) + 1, (i+1)*(N/P));
            // Coger el tiempo de fin
            gettimeofday(&fin, NULL);
            
            tiempo = (fin.tv_sec - inicio.tv_sec)*1e6 + fin.tv_usec - inicio.tv_usec - overhead;
            printf("Resultado do cáculo do fillo PID %d: %f [(tan(sqrt(%d)), (tan(sqrt(%d))]\n",getpid() , resultado, i*(N/P) + 1, (i+1)*(N/P));
            // Calcular el tiempo real de ejecución sin el overhead en este
            imprimirArchivo(SALIDA_ARCHIVO, getpid(), resultado, tiempo); //falta engadir o argumento de tempo cando este feita esa parte
            // Salir del programa para evitar que el hijo itere el bucle y haga su propio fork()
            exit(EXIT_SUCCESS);
        }
        else if (proceso[i] < 0)
        {
            printf("Erro ao crear un novo proceso\n");
            return EXIT_FAILURE;
        }
        else
            continue;
        
    }
    
    // Esperar a que todos los hijos mueran
    for (int i = 0; i < P; i++)
    {
        waitpid(proceso[i], NULL, WUNTRACED);
    }
    

    // Calcular el restante anterior
    proceso[0] = fork();
    if (proceso[0] == 0)
    {
        // Coger los tiempos y calcular
        gettimeofday(&inicio, NULL);
        // Debe ser +1 ya que si no, estaríamos calculando dos veces el número N - (N%P)
        // Ejemplo: N = 11, P = 2 sería [1,5], [6,10] y [N - (N%P), N] es [10, 11]
        resultado = calcularTangente(N - (N%P) + 1, N);
        gettimeofday(&fin, NULL);

        imprimirArchivo(SALIDA_ARCHIVO, getpid(), resultado, (fin.tv_sec - inicio.tv_sec)*1e6 + fin.tv_usec - inicio.tv_usec - overhead);
        // Salir del programa para evitar que el hijo haga su propios fork()
        exit(EXIT_SUCCESS);
    }

    
    proceso[1] = fork();
    if (proceso[1] == 0)
    {
        resultado = 0;
        printf("PID de cálculo do total = %d\n", getpid());
        gettimeofday(&inicio, NULL);
        resultado = calcularTangente(0, N);
        gettimeofday(&fin, NULL);

        // resultado/N para saber la media
        imprimirArchivo(SALIDA_ARCHIVO, getpid(), resultado/N, (fin.tv_sec - inicio.tv_sec)*1e6 + fin.tv_usec - inicio.tv_usec - overhead);
        
        exit(EXIT_SUCCESS);
    }

    proceso[2] = fork();
    if (proceso[2] == 0)
    {
        gettimeofday(&inicio, NULL);
        // P + 1 para recojer al del módulo también
        calcularSuma(SALIDA_ARCHIVO, P + 1, &resultado, &tiempo);
        gettimeofday(&fin, NULL);
        // resultado/N para saber la media
        printf("Resultado total: %f, tempo de procesado: %f\n", resultado/N, tiempo);
        imprimirArchivo(SALIDA_ARCHIVO, getpid(), resultado/N, tiempo);
        exit(EXIT_SUCCESS);
    }
    
    // Esperar a que todos los hijos 0-1 mueran
    for (int i = 0; i < 3; i++)
    {
        waitpid(proceso[i], NULL, WUNTRACED);
    }

    diferenciaValores(SALIDA_ARCHIVO, P + 1, proceso[2]);

    return 0;
}