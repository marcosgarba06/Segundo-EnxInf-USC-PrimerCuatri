#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <unistd.h> 
//Marcos García Blanco e Nicolás Santiago Gómez Forján

#define MAX 1024
int main(int argc, char** argv)
{
	if (argc != 4)
	{
		printf("Erro: número de argumentos incorrecto. Uso ./ejecutable puertoPropio IP puertoDestinatario\n");
		exit(EXIT_FAILURE);
	}
	
	//Definicion de variables
	struct sockaddr_in socketReceptor, socketEmisor;
	socklen_t tamSocket;
	char buffermsg[1024];
	uint16_t puertoPropio, puertoDestinatario;
	int sockEmisor;
	ssize_t bytesEnviados;
	
	puertoPropio = (uint16_t) atoi(argv[1]);
	puertoDestinatario = (uint16_t) atoi(argv[3]);

	tamSocket = sizeof(struct sockaddr_in);

	//Crear socket receptor UDP
	sockEmisor = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockEmisor < 0)
	{
		perror("Erro ao crear o socket.\n");
		exit(EXIT_FAILURE);
	}

	//Configurar struct emisor
	socketEmisor.sin_family = AF_INET; //IPv4
	socketEmisor.sin_port = htons(puertoPropio); //Porto en formato rede
	socketEmisor.sin_addr.s_addr = htonl(INADDR_ANY); //Cualquer IP

	//Configurar struct receptor
	socketReceptor.sin_family = AF_INET; //IPv4
	socketReceptor.sin_port = htons(puertoDestinatario); //Porto en formato rede
	if (inet_pton(AF_INET, argv[2], (void *)&socketReceptor.sin_addr) != 1) //Convertir IP a formato binario e comprobar que se fixo correctamente
	{
		perror("Erro ao convertir a IP a formato binario\n");
		exit(EXIT_FAILURE);
	}

	//Facer bind o socket receptor
	if (bind(sockEmisor, (struct sockaddr *)&socketEmisor, sizeof(struct sockaddr)) < 0)
	{
		perror("Erro ao facer bind do socket.\n");
		close(sockEmisor);
		exit(EXIT_FAILURE);
	}else
	{	
		printf("Socket receptor creado e ligado correctamente. Pode comezar a enviar mensaxes.\n");
		printf("Mensaje a enviar: ");
		scanf(" %[^\n\r]", buffermsg);
		
		//Enviar mensaxe ao receptor
		bytesEnviados = sendto(sockEmisor, buffermsg, strlen(buffermsg), 0, (struct sockaddr*) &socketReceptor, tamSocket);
		if (bytesEnviados < 0)
		{
			perror("Erro ao enviar a mensaxe.\n");
			close(sockEmisor);
			exit(EXIT_FAILURE);
		}else
		{
			printf("Mensaxe enviada correctamente. Bytes enviados: %zd\n", bytesEnviados);
		}
	}
	return 0;	
}