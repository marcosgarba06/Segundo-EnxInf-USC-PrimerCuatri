//Nuria Guerra Casal e Marcos Garcia Blanco

//Compilación: gcc -Wall -o router router.c -lresolv

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <resolv.h>

#define MAX_LINEA  256
#define MAX_RUTAS  100


//Estrutura para a entrada dunha taboa
typedef struct {
    struct in_addr direccion_red; //direccion de rede en formato binario
    int length_prefijo; //lonxitude do pefixo en bits
    int interfaz; //numero de interfaz de saida
} Ruta;

//Parsea unha liña do arquivo de taboa de rutas e extrae a información
int parsea_ruta (char *linea, Ruta *entrada){
    char *posComa;
    char direccion_str[MAX_LINEA];

    //Eliminar salto de fin de liña
    linea[strcspn(linea, "\n")] = 0;
    linea[strcspn(linea, "\r")] = 0;

    //Buscar a coma que separa a rede da interfaz
    posComa = strchr(linea, ',');
    if (posComa == NULL) {
        printf("Erro, formato incorrecto en liña '%s'\n", linea);
        return -1;
    }

    //Separar a parte da rede/profixo interfaz
    *posComa = '\0'; // Reemplazar coma por terminador nulo
    strncpy(direccion_str, linea, MAX_LINEA - 1);
    direccion_str[MAX_LINEA -1] = '\0';

    //Parsear o numero de interfaz
    entrada->interfaz = atoi(posComa + 1);
    
    //Iniciar estructura in_addr a 0
    entrada->direccion_red.s_addr = 0;

    //Convertir rede de formato textual a binario
    // Esta función admite ambas notacions: '193.144.0.0/16' y '193.144/16'
    entrada->length_prefijo = inet_net_pton(AF_INET, direccion_str, (void *)&entrada->direccion_red, sizeof(struct  in_addr));

    if (entrada->length_prefijo < 0){
        printf("Erro, dirección de rede inválida '%s'\n", direccion_str);
        return -1;
    }
    return 0;
}

//Funcion para ler e cargar o arquivo
int cargar_tabla (char *filename, Ruta *rutas, int *num_rutas){
    FILE *archivo;
    char linea[MAX_LINEA];
    int contar = 0;

    archivo = fopen (filename, "r"); //abrimos o archivo para lelo
    if (archivo == NULL){
        printf("Erro ao abrir o arquivo %s\n", filename);
        return -1;
    }
    printf("\nCargando tabla de rutas desde: %s\n\n", filename);

    //Ler liña por liña
    while (fgets(linea, sizeof(linea), archivo) != NULL && contar < MAX_RUTAS) {
        //ignorar liñas baleiras
        if (linea[0] == '\n' || linea[0] == '\r' || linea[0] == '\0') {
            continue;
        }
        if (!parsea_ruta(linea, &rutas[contar])){
            //mostrar información de depuracion
            char red_texto[INET_ADDRSTRLEN + 3];
            if (inet_net_ntop(AF_INET, &rutas[contar].direccion_red, rutas[contar].length_prefijo, red_texto, sizeof(red_texto)) != NULL){
                printf("Ruta %d: %s -> interfaz %d\n", contar +1, red_texto, rutas[contar].interfaz);
            }
            contar++;

        } else{
            printf("Liña ignorada, erro ao lela");
        }
    }

    if (fclose (archivo)){
        printf("\nErro ao pechar o arquivo\n");
    }
    *num_rutas = contar;
    if (!contar){
        printf("\nErro: non se poideron cargar rutas do arquivo\n");
        return -1;
    }

    printf("\nTabla cargada correctamente con %d rutas\n\n", contar);
    return 0;
}

/*
 * Comprueba se unha dirección IP coincide con unha rede dada segundo o numero de bits do prefixo.
 * Parámetros:
 *   ip: dirección IP destino (en orden de rede.
 *   red: dirección de rede (en orden de rede).
 *   length_prefijo: número de bits a comparar desde o inicio.
 * Retorno:
 *   1 si coincide, 0 si non.
 */
int comprobar_coincidencia_prefijo(uint32_t ip, uint32_t red, int length_prefijo){
    uint32_t mascara;

    if (length_prefijo == 0){
        return 1; //Cualquier IP coincide con unha rede /0 (red por defecto)
    }

    //se crea la máscara, los length_prefijo bits mñas significativos tienen que estar a 1, el resto a 0
    //mascara = htonl(~((1U << (32 - length_prefijo)) - 1));
   mascara = htonl(~((1U << (32 - length_prefijo)) - 1));
    
    //Compara los bits del prefijo
    return (ip & mascara) == (red & mascara);
}

/*
 *  Aplica a regla do prefixo máis longo sobre o conxunto de rutas cargadas.
 * Parámetros:
 *   ip_destino: dirección IP destino (struct in_addr).
 *   rutas: array de rutas dispoñibles.
 *   num_rutas: número de entradas válidas cargadas.
 *   mejor_interfaz: puntero onde se almacenará a interface seleccionada.
 *   mejor_prefijo: puntero onde se almacenará a lonxitude do prefixo seleccionado.
 * Retorno:
 *   0 sempre.
 */
void encontrar_mejor_ruta(struct in_addr ip_destino, Ruta *rutas, int num_rutas, int *mejor_interfaz, int *mejor_prefijo) {
    *mejor_prefijo = -1; // Usa -1 para indicar "ningunha coincidencia atopada"
    *mejor_interfaz = 0; // La interfaz por defecto según el enunciado

    // Buscar la ruta con el prefijo más largo que coincida 
    for (int i = 0; i < num_rutas; i++) {
        if (comprobar_coincidencia_prefijo(ip_destino.s_addr, rutas[i].direccion_red.s_addr, rutas[i].length_prefijo)) {
            
            // Si la ruta actual que coincide tiene un prefijo más largo que la mejor que teníamos,
            // esta pasa a ser la nueva mejor ruta.
            if (rutas[i].length_prefijo > *mejor_prefijo) {
                *mejor_prefijo = rutas[i].length_prefijo;
                *mejor_interfaz = rutas[i].interfaz;
            }
        }
    }
    
    // Si, tras recorrer todas las rutas, no se encontró ninguna coincidencia (mejor_prefijo sigue en -1),
    // se asigna explícitamente el prefijo de la ruta por defecto (0).
    if (*mejor_prefijo == -1) {
        *mejor_prefijo = 0;
    }
}


int main (int argc, char* argv[]){
    if (argc != 3){
        printf("Uso: %s <arquivo> <direccion IP>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *filename = argv[1];
    char *ip = argv[2];
    Ruta rutas[MAX_RUTAS];
    int num_ruta = 0;

    //Cargar a taboa de enrutamento
    if(cargar_tabla(filename, rutas, &num_ruta)){
        return EXIT_FAILURE;
    }

    // Convertir IP de entrada a formato binario
    struct in_addr ip_destino;
    if (inet_pton(AF_INET, ip, &ip_destino) != 1) {
        printf("Erro, dirección IP inválida\n");
        return EXIT_FAILURE;
    }

    //Buscar a mellor ruta
    int mejor_interfaz;
    int mejor_prefijo;
    encontrar_mejor_ruta(ip_destino, rutas, num_ruta, &mejor_interfaz, &mejor_prefijo);
    

    //Convertir a IP ao formato textual
    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &ip_destino, ip_str, INET_ADDRSTRLEN) == NULL){
        printf("Erro ao convertir a IP a texto\n");
        return EXIT_FAILURE;
    }

    //Mostrar resultados
    printf("Dirección IP destino: %s\n", ip_str);
    printf("Interfaz de salida: %d\n", mejor_interfaz);
    printf("Lonxitude do prefijo: %d bits\n", mejor_prefijo);

    return EXIT_SUCCESS;
}