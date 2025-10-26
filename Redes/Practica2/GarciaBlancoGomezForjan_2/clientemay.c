#include <stdint.h>
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

//Marcos García Blanco e Nicolás Santiago Gómez Forján

char* arquivoaMaiusculas(char* nome)
{
    // Comezar analizando o PATH de arquivo polo final xa que é onde vai a estar o nome
    for (int i = (int) strlen(nome); i > -1; i--) 
    {
        // Mirar se pasámonos algún símbolo de indicación de directorio e parar 
        // xa que comezaríamos a renomear os directorios do path
        if (nome[i] == '\\' ||nome[i] == '/')
        {
            break;
        }
        // Pasar a letra correspondende a maíusculas
        nome[i] = (char) toupper(nome[i]);
    }

    return nome;
}

int main(int argc, char **argv)
{
    // Comprobar que o número de argumentos é correcto
    if(argc != 4)
    {
        printf("Erro: número de argumentos incorrecto\n");
        return EXIT_FAILURE;
    }

    // Crear o socket e comprobar que se creou correctamente
    int nsocketRecivir = socket(AF_INET, SOCK_STREAM, 0);
    if (nsocketRecivir == -1) 
    {
        printf("Erro ao crear os sockets\n");
        return EXIT_FAILURE;
    }

    // Crear a estrutura do socket a usar para o socket
    struct sockaddr_in socketEnviar;
    socketEnviar.sin_family = AF_INET;
    socketEnviar.sin_port = htons((uint16_t) atoi(argv[3]));
    if (inet_pton(AF_INET, argv[2], (void*) &socketEnviar.sin_addr) != 1)
    {
        perror("Erro convertindo o IP a formato binario:");
        return EXIT_FAILURE;
    }

    FILE* arquivoLectura, *arquivoEscritura;

    // Abrir os arquivos e ver se son avertos correctamente
    if ((arquivoLectura = fopen(argv[1], "r")) != NULL && (arquivoEscritura = fopen(arquivoaMaiusculas(argv[1]), "w+")) != NULL)
    {
        char linha[1024];

        // Conectarse ao servidor
        if (connect(nsocketRecivir, (struct sockaddr *) &socketEnviar, sizeof(socketEnviar)) == -1)
        {
            perror("Erro ao conectarse ao servidor");
            close(nsocketRecivir);
            return EXIT_FAILURE;
        }

        printf("Cliente conectado a %s\n", argv[2]);

        while (fgets(linha, sizeof(linha), arquivoLectura) != NULL) 
        {
            // Enviar os datos ao servidor
            if(send(nsocketRecivir, linha, (size_t) strlen(linha), 0) != (ssize_t) strlen(linha))
                printf("Erro enviado os datos ao servidor\n");

            // Recivir os datos menos o último posterio, que poñemos cun \0 para que sexa con fin nulo
            ssize_t nrecv;
            if((nrecv = recv(nsocketRecivir, linha, sizeof(linha) - 1, 0)) <= 0)
                printf("Erro ao recivir os datos do servidor\n");
            
            linha[nrecv] = '\0';

            fprintf(arquivoEscritura, "%s", linha);
        }

        // Pechar os socket
        close(nsocketRecivir);

        // Pechar os arquivos abertos
        fclose(arquivoEscritura);
        fclose(arquivoLectura);
    }
    else 
    {
        printf("Erro ao abrir o arquivo de entrada ou de saída\n");
        close(nsocketRecivir);
        return EXIT_FAILURE;
    }

    close(nsocketRecivir);

    return 0;
}