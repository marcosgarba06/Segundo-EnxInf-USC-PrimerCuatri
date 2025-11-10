//Nuria Guerra Casal e Marcos Garcia Blanco

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LINEA  256
#define MAX_RUTAS  100

//Estrutura para a entrada dunha taboa
typedef struct {
    struct in_addr direccion_red; //direccion de rede en formato binario
    int length_prefijo; //lonxitude do pefixo en bits
    int interfaz; //numero de interfaz de saida
} Ruta;

//Parsea linea da taboa
int parsea_ruta (char *linea, Ruta *entrada){
    char *token;
    char *posComa;
    char direccion_str[MAX_LINEA];

    //Eliminar salto de liña
    linea[strcspn(linea, "\n")] = 0;
    linea[strcspn(linea, "\r")] = 0;

    //Buscar a coma que separa a rede da interfaz */
    posComa = strchr(linea, ',');
    if (posComa == NULL) {
        printf("Error: formato incorrecto en línea '%s'\n", linea);
        return -1;
    }

    //Separar a parte da rede/profixo interfaz
    *posComa = '\0';
    strncpy(direccion_str, linea, MAX_LINEA - 1);
    direccion_str[MAX_LINEA -1] = '\0';

    //Parsear o numero de interfaz
    entrada->interfaz = atoi(posComa +1);
    
    //Iniciar estructura in_addr a 0
    entrada->direccion_red.s_addr = 0;

    //Convertir rede de formato textual a binario
    entrada->length_prefijo = inet_net_pton(AF_INET, direccion_str, (void *)&entrada->direccion_red, sizeof(struct  in_addr));

    if (entrada->length_prefijo < 0){
        printf("Error: dirección de red inválida '%s'\n", direccion_str);
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
        printf("Error ao abrir o arquivo %s\n", filename);
        return -1;
    }

    //Ler liña por liña
    while (fgets(linea, sizeof(linea), archivo) != NULL && contar < MAX_RUTAS) {
        //ignorar liñas baleiras
        if (linea[0] == '\n' || linea[0] == '\r') {
            continue;
        }
        if (!parsea_ruta(linea, &rutas[contar])){
            contar++;
        } else{
            printf("Liña ignorada, erro ao lela");
        }
    }

    if (fclose (archivo)){
        printf("Erro ao pechar o arquivo\n");
    }
    *num_rutas = contar;
    if (!contar){
        printf("Erro: non se poideron cargar rutas do arquivo\n");
        return -1;
    }
    return 0;
}

int verificarCoincidenciaPrefijo(uint32_t ip, uint32_t red, int longitud_prefijo){}

int main (int argc, char* argv[]){
    if (argc != 3){
        printf("Uso: %s <archivo> <direccion IP>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *filename = argv[1];
    char *ip = argv[2];

    // Convertir IP de entrada a formato binario
    struct in_addr ip_destino;
    if (inet_pton(AF_INET, ip, &ip_destino) != 1) {
        printf("Error: dirección IP inválida\n");
        return EXIT_FAILURE;
    }
}