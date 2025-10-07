#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

// Convierte el nombre del archivo a mayúsculas (solo el nombre, no el path)
char* arquivoaMaiusculas(char* nome)
{
    for (int i = strlen(nome) - 1; i >= 0; i--)
    {
        // Detenerse si encuentra un separador de directorio
        if (nome[i] == '\\' || nome[i] == '/')
            break;

        nome[i] = toupper(nome[i]);
    }

    return nome;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Uso: %s <arquivo> <ip_servidor> <porto>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Crear socket TCP
    int nsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (nsocket == -1)
    {
        perror("Erro ao crear o socket");
        return EXIT_FAILURE;
    }

    // Configurar dirección del servidor
    struct sockaddr_in socketServidor;
    socketServidor.sin_family = AF_INET;
    socketServidor.sin_port = htons(atoi(argv[3]));

    if (inet_pton(AF_INET, argv[2], &socketServidor.sin_addr) != 1)
    {
        perror("Erro ao convertir a dirección IP");
        close(nsocket);
        return EXIT_FAILURE;
    }

    // Conectarse al servidor
    if (connect(nsocket, (struct sockaddr*)&socketServidor, sizeof(socketServidor)) == -1)
    {
        perror("Erro ao conectarse ao servidor");
        close(nsocket);
        return EXIT_FAILURE;
    }

    printf("Conectado ao servidor %s:%s\n", argv[2], argv[3]);

    // Abrir archivo de lectura y escritura
    FILE *arquivoLectura = fopen(argv[1], "r");
    if (arquivoLectura == NULL)
    {
        perror("Erro ao abrir o arquivo de entrada");
        close(nsocket);
        return EXIT_FAILURE;
    }

    char nomeSaida[256];
    strncpy(nomeSaida, argv[1], sizeof(nomeSaida));
    nomeSaida[sizeof(nomeSaida) - 1] = '\0';
    FILE *arquivoEscritura = fopen(arquivoaMaiusculas(nomeSaida), "w");
    if (arquivoEscritura == NULL)
    {
        perror("Erro ao crear o arquivo de saída");
        fclose(arquivoLectura);
        close(nsocket);
        return EXIT_FAILURE;
    }

    // Búfer para enviar/recibir
    char linha[1024];
    ssize_t nbytes;

    // Leer línea a línea del archivo
    while (fgets(linha, sizeof(linha), arquivoLectura) != NULL)
    {
        size_t len = strlen(linha);

        // Enviar al servidor
        if (send(nsocket, linha, len, 0) != (ssize_t)len)
        {
            perror("Erro ao enviar datos");
            break;
        }

        // Recibir respuesta del servidor
        nbytes = recv(nsocket, linha, sizeof(linha) - 1, 0);
        if (nbytes <= 0)
        {
            perror("Erro ao recibir datos do servidor");
            break;
        }

        linha[nbytes] = '\0'; // Asegurar que es cadena válida

        // Escribir en el archivo de salida
        fputs(linha, arquivoEscritura);
    }

    printf("Transferencia completada. Arquivo xerado: %s\n", nomeSaida);

    // Cerrar todo
    fclose(arquivoLectura);
    fclose(arquivoEscritura);
    close(nsocket);

    return 0;
}
