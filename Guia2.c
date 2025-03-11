#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PUERTO 8000

// Funciones para conversión
float convertir_a_fahrenheit(float celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

float convertir_a_celsius(float fahrenheit) {
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

void manejar_error(const char *mensaje) {
    perror(mensaje);
    exit(1);
}

int main() {
    int socket_servidor, socket_cliente;
    struct sockaddr_in direccion_servidor;

    // Creación del socket
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_servidor < 0)
        manejar_error("Error al crear socket");

    memset(&direccion_servidor, 0, sizeof(direccion_servidor));
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(PUERTO);

    // Enlazar el socket
    if (bind(socket_servidor, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0)
        manejar_error("Error en bind");

    // Escuchar conexiones
    listen(socket_servidor, 5);
    printf("Servidor en espera en el puerto %d...\n", PUERTO);

    while (1) {
        socket_cliente = accept(socket_servidor, NULL, NULL);
        if (socket_cliente < 0) {
            perror("Error en la conexión");
            continue;
        }
        printf("Cliente conectado.\n");

        int activo = 1;
        while (activo) {
            char buffer[256];
            int bytes_recibidos = read(socket_cliente, buffer, sizeof(buffer) - 1);
            if (bytes_recibidos <= 0) break;

            buffer[bytes_recibidos] = '\0';

            char *token = strtok(buffer, "/");
            int opcion = atoi(token);
            
            if (opcion == 0) {
                activo = 0;
            } else {
                token = strtok(NULL, "/");
                float valor = atof(token);
                float resultado = 0;
                char respuesta[64];

                if (opcion == 1) {
                    resultado = convertir_a_fahrenheit(valor);
                    sprintf(respuesta, "%.2f°C → %.2f°F", valor, resultado);
                } 
                else if (opcion == 2) {
                    resultado = convertir_a_celsius(valor);
                    sprintf(respuesta, "%.2f°F → %.2f°C", valor, resultado);
                } 
                else {
                    sprintf(respuesta, "Código %d no reconocido", opcion);
                }

                write(socket_cliente, respuesta, strlen(respuesta));
            }
        }
        close(socket_cliente);
        printf("Cliente desconectado.\n");
    }

    close(socket_servidor);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PUERTO 8000

void manejar_error(const char *mensaje) {
    perror(mensaje);
    exit(1);
}

int main() {
    int socket_cliente;
    struct sockaddr_in direccion_servidor;
    struct hostent *servidor;

    // Crear socket
    socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cliente < 0)
        manejar_error("Error al abrir socket");

    servidor = gethostbyname("127.0.0.1");
    if (servidor == NULL) {
        fprintf(stderr, "Error, host no encontrado\n");
        exit(0);
    }

    memset(&direccion_servidor, 0, sizeof(direccion_servidor));
    direccion_servidor.sin_family = AF_INET;
    memcpy(&direccion_servidor.sin_addr.s_addr, servidor->h_addr, servidor->h_length);
    direccion_servidor.sin_port = htons(PUERTO);

    // Conectar con el servidor
    if (connect(socket_cliente, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0)
        manejar_error("Error al conectar");

    char mensaje[64];
    
    // Enviar conversión Celsius a Fahrenheit
    strcpy(mensaje, "1/25.0");
    write(socket_cliente, mensaje, strlen(mensaje));

    char buffer[256];
    int bytes_leidos = read(socket_cliente, buffer, 255);
    buffer[bytes_leidos] = '\0';
    printf("Servidor responde: %s\n", buffer);

    // Enviar conversión Fahrenheit a Celsius
    strcpy(mensaje, "2/77.0");
    write(socket_cliente, mensaje, strlen(mensaje));

    bytes_leidos = read(socket_cliente, buffer, 255);
    buffer[bytes_leidos] = '\0';
    printf("Servidor responde: %s\n", buffer);

    // Enviar mensaje de desconexión
    strcpy(mensaje, "0/");
    write(socket_cliente, mensaje, strlen(mensaje));

    close(socket_cliente);
    return 0;
}

