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

int main(int argc, char const *argv[])
{
    //Definicion de variables

    // Comprobar que se ha pasado  un numero de argumentos correcto (IP y puerto)
    if (argc != 3)
    {
        printf("Introducir IP y puerto como argumentos\n");
        exit(EXIT_FAILURE);
    }
    //Copiar IP en una variable
    char ip[INET_ADDRSTRLEN];
    strncpy(ip, argv[1], INET_ADDRSTRLEN); //Copia la IP en la variable ip

    //Convertir IP de texto a binario ya que por comandos sale en formato de cadena de caracteres
    struct in_addr ipBinario;

    //Hace la conversion y comprueba que se ha hecho correctamente 
    if (inet_pton(AF_INET, ip, (void *)&ipBinario.s_addr) != 1) 
    {
        perror("Error al convertir la IP a formato binario\n");
        exit(EXIT_FAILURE);
    }
    //printf("IP en formato binario: %X\n", ipBinario.s_addr);

    // Convertir puerto a entero y almaceralo en tipo de entero entre 0 y 65535
    // ya que el puerto solo puede estar en este rango de valore
    uint16_t puerto;
    puerto = (uint16_t)atoi(argv[2]);


    //Crear socket cliente TCP
    int socketClient;
    socketClient = socket(AF_INET, SOCK_STREAM, 0); 
    //AF_INET para IPv4, SOCK_STREAM para TCP
    
    if (socketClient < 0)
    {
        perror("Error al crear el socket.\n");
        exit(EXIT_FAILURE);
    }

    //Configuración de la direccion del servidor
    struct sockaddr_in direccionServidor; 

   //Configuracion struct sockaddr_in
    uint16_t puertoFormatRed = htons(puerto); //Convertir puerto a formato red

    direccionServidor.sin_family = AF_INET; //Direccion IPv4
    direccionServidor.sin_port = puertoFormatRed; //Puerto en formato red
    direccionServidor.sin_addr = ipBinario; //Direccion IP del servidor

    //Conectar al servidor usando la funcion connect
    if (connect(socketClient, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) != 0) 
    //Se conecta y comprueba si hay error si lo hay se cierra el socket y se sale del programa
    {
        perror("Error al conectar con el servidor.\n");
        close(socketClient); 
        exit(EXIT_FAILURE);
    }

    //Variable para almacenar el mensaje recibido del servidor
    char mensajeRecibido[1024];
    sleep(10); //Esperar 1 segundo para asegurar que el servidor esta listo
    
    //Recepcion de los datos del servidor
    ssize_t n;
    size_t bytesMensaje = 1000;
    n = recv(socketClient, mensajeRecibido, bytesMensaje, 0);
    if (n < 0)
    {
        perror("Error al recibir datos del servidor.\n");
    }else if (n == 0)
    {
        printf("El servidor ha cerrado la conexion.\n");
    }else
    {
        mensajeRecibido[n] = '\0'; //Asegurar que el mensaje recibido es una cadena de caracteres
        printf("Mensaje recibido del servidor: %s\n", mensajeRecibido);
        printf("Bytes recibidos: %zd\n", n);
    }

    //Código alternativo para recibir mensajes con un while para el apartado 1 d.
    /*char mensaje[1024];
    ssize_t n;
    size_t bytesMensaje = 10; //10 bytes a recibir

    while ((n = recv(socketClient, mensaje, bytesMensaje, 0)) > 0) {
        mensaje[n] = '\0';
        printf("Mensaje recibido del servidor: %s\n", mensaje);
        printf("Bytes recibidos: %zd\n", n);
    }

    if (n < 0) {
        perror("Error al recibir el mensaje");
    }*/

    close(socketClient); //Cerrar el socket cliente ya que no se necesita mas
    printf("\nEl servidor ha cerrado la conexion.\n");
    return 0;
}
