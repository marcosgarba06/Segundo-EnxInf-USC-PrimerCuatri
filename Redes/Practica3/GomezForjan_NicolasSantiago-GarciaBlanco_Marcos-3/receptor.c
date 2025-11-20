#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define MIN 8

int main(int argc, char** argv)
{
	// Comprobar que hai polo menos un argumento
	if (argc < 2)
	{
		printf("Erro: cantidade insuficiente de argumentos\n");
		return EXIT_FAILURE;
	}

	// Crear o socket e comprobar que se creou correctamente
	int nsocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (nsocket < 0)
	{
		perror("Erro creando o socket");
		return EXIT_FAILURE;
	}

	// Crear a estrutura do socket e recheala con datos
	struct sockaddr_in socket;
	socket.sin_addr.s_addr = htonl(INADDR_ANY);
	socket.sin_family = AF_INET;
	socket.sin_port = htons(atoi(argv[1]));

	if (bind(nsocket, (struct sockaddr*) &socket, (socklen_t) sizeof(socket)) < 0)
	{
		perror("Erro ao facer bind do socket");
		close(nsocket);
		return EXIT_FAILURE;
	}

	socklen_t tamsocket = sizeof(socket);

	char mensaxe[MAX];

	// Recivir a mensaxe
	printf("Tentando recivir a mesaxe\n");
	ssize_t nbytes = recvfrom(nsocket, mensaxe, (size_t) MAX, 0, (struct sockaddr*) &socket, &tamsocket);

	char ipRecividoTexto[16];
	uint16_t ipRecividoBinario = ntohs(socket.sin_addr.s_addr);

	inet_ntop(AF_INET, &ipRecividoBinario, ipRecividoTexto, sizeof(ipRecividoTexto));

	printf("Mesaxe recivida dende %s:%s: %s (%ld bytes)\n", ipRecividoTexto, argv[1], mensaxe, nbytes);

	// Apartado 2

	close(nsocket);
	
	return EXIT_SUCCESS;
}