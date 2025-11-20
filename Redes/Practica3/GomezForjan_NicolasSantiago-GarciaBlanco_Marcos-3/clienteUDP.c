#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <unistd.h> 
#include <ctype.h>

#define MAX 1024
//Marcos García Blanco e Nicolás Santiago Gómez Forján

int main(int argc, char** argv)
{
    //Comprobar argumentos por comandos
    if (argc != 5)
    {
        printf("Erro: número de argumentos incorrecto.\n");
        printf("Uso: ./clienteUDP ficheroEntrada puertoPropio IPServidor puertoServidor\n");
        exit(EXIT_FAILURE);
    }
    
    char *nameFicheroEntrada = argv[1];
    char *ipServidor = argv[3];
    uint16_t puertoPropio = (uint16_t) atoi(argv[2]);
    uint16_t puertoServidor = (uint16_t) atoi(argv[4]);

    if (puertoPropio < IPPORT_RESERVED || puertoServidor < IPPORT_RESERVED)
    {
        printf("Erro: os portos deben ser maiores que %d\n", IPPORT_RESERVED);
        exit(EXIT_FAILURE);
    }

    //Abrir fichero de entrada
    FILE *ficheroEntrada = fopen(nameFicheroEntrada, "r");
    if (ficheroEntrada == NULL)
    {
        perror("Erro abrindo o ficheiro de entrada");
        exit(EXIT_FAILURE);
    }

    // Cambiar a mayusculas el nombre del archivo para tener el archivo de salida en mayusculas
    char nameFicheroSalida[MAX];
    int i;
    for (i = 0; nameFicheroEntrada[i] != '\0'; i++)
    {
        nameFicheroSalida[i] = toupper(nameFicheroEntrada[i]);
    }
    nameFicheroSalida[i] = '\0';

    // Abrir fichero de salida
    FILE *ficheroSalida = fopen(nameFicheroSalida, "w");
    if (ficheroSalida == NULL)
    {
        perror("Erro creando o ficheiro de saída en mayúsculas");
        fclose(ficheroEntrada);
        exit(EXIT_FAILURE);
    }
    printf("Ficheiro de saída: %s\n", nameFicheroSalida);

    // Crear socket UDP
    int sockCliente = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockCliente < 0)
    {
        perror("Erro ao crear o socket UDP");
        fclose(ficheroEntrada);
        fclose(ficheroSalida);
        exit(EXIT_FAILURE);
    }

    char linea[MAX];
    char buffer[MAX];
    ssize_t bytes;
    struct sockaddr_in ipportserv, ipportcli, ipportresp;
    socklen_t tam = sizeof(ipportserv);

    // Configurar socket do cliente (bind con porto propio)
    ipportcli.sin_family = AF_INET;
    ipportcli.sin_port = htons(puertoPropio);
    ipportcli.sin_addr.s_addr = htonl(INADDR_ANY);

    // Hacer bind del socket cliente y comprobar si se ha hecho correctamente
    if (bind(sockCliente, (struct sockaddr *)&ipportcli, sizeof(ipportcli)) < 0)
    {
        perror("Erro ao facer bind do socket cliente");
        close(sockCliente);
        fclose(ficheroEntrada);
        fclose(ficheroSalida);
        exit(EXIT_FAILURE);
    }
    
    // Configurar socket del servidor
    ipportserv.sin_family = AF_INET;
    ipportserv.sin_port = htons(puertoServidor);
    if (inet_pton(AF_INET, ipServidor, &ipportserv.sin_addr) != 1)
    {
        perror("Erro ao converter a IP do servidor a formato binario");
        close(sockCliente);
        fclose(ficheroEntrada);
        fclose(ficheroSalida);
        exit(EXIT_FAILURE);
    }
    
    // Mensaxes informativas de conexion
    printf("Cliente UDP iniciado correctamente\n");
    printf("Porto propio: %u\n", puertoPropio);
    printf("Enviando liñas a %s:%u\n\n", ipServidor, puertoServidor);

    //Bucle para ler liñas do ficheiro de entrada, envialas ao servidor e recibilas de volta
    while (fgets(linea, sizeof(linea), ficheroEntrada) != NULL)
    {
        //Calcular lonxitude da liña
        size_t len = strlen(linea);

        //Enviar liña ao servidor utilizando sendto (CON el \n)
        bytes = sendto(sockCliente, linea, len, 0, (struct sockaddr*)&ipportserv, tam);

        //Comprobar se a liña foi enviada correctamente
        if (bytes < 0)
        {
            perror("Erro ao enviar a liña ao servidor");
            break;
        }

        printf("Enviados %zd bytes\n", bytes);

        //Recibir liña do servidor utilizando recvfrom
        socklen_t tamresp = sizeof(ipportresp);
        bytes = recvfrom(sockCliente, buffer, MAX - 1, 0, (struct sockaddr*)&ipportresp, &tamresp); 

        //Comprobar se a liña foi recibida correctamente
        if (bytes < 0)
        {
            perror("Erro ao recibir datos do servidor");
            break;
        }
        
        buffer[bytes] = '\0';
        printf("Recibidos %zd bytes\n\n", bytes);
        fprintf(ficheroSalida, "%s", buffer);
    }

    printf("Finalizando o cliente UDP...\n");
    
    //Pechar ficheiros e socket
    close(sockCliente);
    fclose(ficheroEntrada);
    fclose(ficheroSalida);
    
    return EXIT_SUCCESS;
}