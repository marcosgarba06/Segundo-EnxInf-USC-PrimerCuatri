#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

//Marcos García Blanco y Nicolás Santiago Gómez Forján

int main(int argc, char** argv)
{
    // Comprobar o nº de argumentos
    if (argc == 2)
    {
        // Asignar o número de porto segundo a línea de comandos
        int nporto = atoi(argv[1]);
        // Crear o socket
        int nsocket = socket(AF_INET, SOCK_STREAM, 0);

        if (nsocket > 0)
        {
            // Crear a direción do socket e asignar os valores correspondentes
            struct sockaddr_in ipportserv;
            ipportserv.sin_family = AF_INET;
            ipportserv.sin_addr.s_addr = htonl(INADDR_ANY);
            ipportserv.sin_port = htons((u_int16_t) nporto);

            // Facer bind do socket coa súa dirección
            if (bind(nsocket, (struct sockaddr*) &ipportserv, sizeof(struct sockaddr)) > -1)
            {
                // Activar o socket para escoita
                if (listen(nsocket, 2) != -1)
                {
                    printf("Servidor escoitando\n");
                    // Calcular o tamaño da estrutura do ip
                     
                    socklen_t tamip = sizeof(ipportserv);
                    // Crear a conexión do socket
                    int sockcon = accept(nsocket, (struct sockaddr*) &ipportserv, &tamip);
                    if (sockcon > -1)
                    {
                        printf("Servidor conectado ao cliente\n");
                        char ipTexto[32];
                        inet_ntop(AF_INET, &ipportserv.sin_addr.s_addr, ipTexto, sizeof(ipTexto));
                        printf("IP conectada: %s\n", ipTexto);

                        // Crear as mesaxes para enviar dende o servidor
                        char msx1[] = "Mesaxe xenérica 1\n";
                        char msx2[] = "Vivo nas túas paredes\n";

                        // Enviar a mesaxe 1 con comprovación de erros
                        ssize_t nbites = send(sockcon, msx1, strlen(msx1), 0);
                        if(nbites != strlen(msx1))
                        {
                            printf("Erro ao enviar a mesaxe 1\n");
                        }
                        sleep(1);
                        // Enviar a mesaxe 2 con comprovación de erros
                        nbites = send(sockcon, msx2, strlen(msx2), 0);
                        if(nbites != strlen(msx2))
                        {
                            printf("Erro ao enviar a mesaxe 2\n");
                        }

                        close(nsocket);
                        printf("Socket pechado\n");
                    }
                    else
                    {
                        perror("Erro ao tentar poñer en escoita o socket\n");
                        return (EXIT_FAILURE);
                    }
                    
                }
                else
                {
                    perror("Erro ao poñer en escoita o porto\n");
                    return (EXIT_FAILURE);
                }
                
            }
            else
            {
                perror("Erro ao facer bind no socket\n");
                return (EXIT_FAILURE);
            }
            
        }
        else
        {
            perror("Erro ao crear o socket\n");
            return (EXIT_FAILURE);
        }
    }
    else
    {
        printf("Número de argumentos inválido\n");
        return (EXIT_FAILURE);
    }
}