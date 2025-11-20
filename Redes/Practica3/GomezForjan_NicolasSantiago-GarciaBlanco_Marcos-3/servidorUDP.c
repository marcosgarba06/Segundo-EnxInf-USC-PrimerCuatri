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

#define TAM_BUFFER 1024

//Marcos García Blanco e Nicolás Santiago Gómez Forján

int main(int argc, char** argv)
{
    if (argc < 2)
	{
		printf("Erro: cantidade insuficiente de argumentos\n");
        printf("Uso: <porto local>\n");
		return EXIT_FAILURE;
	}

    // Crear os socket
    int nsocketRecivir;
    nsocketRecivir = socket(AF_INET, SOCK_DGRAM, 0);

    // Comprobar que os socket foron creados correctamente
    if (nsocketRecivir < 0)
    {
        perror("Erro ao crear os sockets");
        return EXIT_FAILURE;
    }
    
    // Crear o socket para recivir
    struct sockaddr_in socketRecivir;
    socketRecivir.sin_family = AF_INET;
    socketRecivir.sin_addr.s_addr = htonl(INADDR_ANY);
    socketRecivir.sin_port = htons((uint16_t) atoi(argv[1]));

    // Ligalo o socket receptor
    if (bind(nsocketRecivir, (struct sockaddr*) &socketRecivir, sizeof(socketRecivir)) < 0)
    {
        perror("Erro ao facer bind");
        return EXIT_FAILURE;
    }

    // Declarar as variables usadas para recivir
    char buf[TAM_BUFFER];
    ssize_t nbytes = 0;
    struct sockaddr_in socketEnviar;
    socklen_t tamSocketEnviar = sizeof(socketEnviar);

    printf("Servidor escoitando...\n");
    // Recivir os bytes
    while((nbytes = recvfrom(nsocketRecivir, buf, sizeof(buf), 0, (struct sockaddr*) &socketEnviar, &tamSocketEnviar)) > -1)
    {
        // Asegurarse que a cadea acaba en \0 para strlen()
        buf[nbytes] = '\0';
        // Pasar os datos a maiúsculas
        for (int i = 0; i < (int) strlen(buf); i++)
        {
            buf[i] = (char) toupper(buf[i]);
        }
        
        // Enviar os datos ao cliente e verificar e foron enviados correctamente
        printf("Mesaxe a enviar: %s", buf);
        if (sendto(nsocketRecivir, buf, nbytes, 0, (struct sockaddr*) &socketEnviar, sizeof(socketEnviar)) < 0)
            printf("\nErro ao enviar a mesaxe\n");
        else
        {
            char ipString[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &socketEnviar.sin_addr.s_addr, ipString, sizeof(ipString));

            printf("\nDatos enviados a %s:%u\n", ipString, ntohs(socketRecivir.sin_port));
        }
        printf("-------------------------------\n");
    }

    close(nsocketRecivir);

    return EXIT_SUCCESS;
}