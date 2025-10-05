#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Introducir puerto como argumento\n");
        exit(EXIT_FAILURE);
    }

    //definicion de variables
    uint16_t puerto;
    int socketServidor, socketDatos, tamanho;
    struct sockaddr_in ipportserv, datos;
    char ipCliente[INET_ADDRSTRLEN]; //ip del cliente


    puerto = (uint16_t)atoi(argv[1]); //Convertir el puerto de caracteres a enteros de tipo uint16_t

    socketServidor = socket(AF_INET, SOCK_STREAM, 0); //Crear socket TCP

    if (socketServidor < 0)
    {
        perror("Error al crear el socket.\n");
        exit(EXIT_FAILURE);
    }
    

    //configuracion struct
    ipportserv.sin_family = AF_INET;
    ipportserv.sin_addr.s_addr = htonl(INADDR_ANY);
    ipportserv.sin_port = htons(puerto);
    
    //hacer bind para asignar direccion al socket
    if(bind(socketServidor, (struct sockaddr*)&ipportserv, sizeof(struct sockaddr)) < 0)
    {
        perror("Error en bind.\n");
        close(socketServidor);
        exit(EXIT_FAILURE);
    }

    //Poner el socket en modo escucha y comprobar que no hay errores
    if(listen(socketServidor, 2) < 0)
    {
        perror("Error en listen.\n");
        close(socketServidor);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando...\n");
   
    while (1)
    {
        tamanho = sizeof(struct sockaddr_in);
        socketDatos = accept(socketServidor, (struct sockaddr *)&datos, &tamanho); //Aceptar conexiones entrantes
    
        if(socketDatos < 0) //comprobar que no hay errores en accept
        { 
            perror("Error en accept.\n");
            close(socketServidor);
            exit(EXIT_FAILURE);
        }
        
        inet_ntop(AF_INET, &(datos.sin_addr), ipCliente, INET_ADDRSTRLEN); //pasa la ip recibida en binario a formato textual
        printf("Dirección IP cliente conectado: %s:%d\n", ipCliente, ntohs(datos.sin_port));
        //ntohs para converir del entero de orden de red a entero de orden de host

        char linea[1024];
        int n, msgEnv;

        //Recibir datos del cliente hasta que se cierre la conexion
        while ((n = recv(socketDatos, linea, sizeof(linea) - 1, 0)) > 0)
        {
            //n = numero de bytes recibidos -1
            linea[n] = '\0'; //Se añade el caracter nulo al final de la cadena recibida para asegurar que es una cadena
            
            //Pasar a mayusculas
            for (int i = 0; i < n; i++){
                linea[i] = toupper(linea[i]);
            }

            //Enviar datos al cliente
            msgEnv = send(socketDatos, linea, n, 0);

            if (msgEnv < 0) //comprobar que no hay errores en send
            {
                perror("Error en send.\n");
                close(socketDatos);
                close(socketServidor);
                exit(EXIT_FAILURE);
            }
            
        }
            if (n<0) //comprobar que no hay errores en recv
            {
                perror("Error en recv.\n");
                close(socketDatos);
                close(socketServidor);
                exit(EXIT_FAILURE);
            }
            
            close(socketDatos); //Cerrar el socket de datos
            printf("Datos enviados al cliente.\n");
            printf("Cliente desconectado: %s:%d\n", ipCliente, ntohs(datos.sin_port));

            printf("\n-------------------------------------\n");
            printf("Puerto: %d\n", puerto);
            printf("Servidor escuchando...\n");
    }
    close(socketServidor); //Cerrar el socket del servidor
    return 0;
}