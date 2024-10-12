//// ESTE SCRIPT MUESTRA COMO INTERACTUAR CON VARIOS COMANDOS SERIALMENTE MEDIANTE UNA CONSOLA
// COMANDOS SERIALES

// Comando MOTOR <dirección>
// Cambia la dirección de giro del motor, lo que a su vez cambia el sentido del flujo del canal.
// MOTOR LEFT: Establece el motor para que gire hacia la izquierda (flujo de derecha a izquierda).
// MOTOR RIGHT: Establece el motor para que gire hacia la derecha (flujo de izquierda a derecha).

// POSICIONES
// Para los botes se manejan una cola con un orden específico para cada bote generado, cada cola
// tiene capacidad para 3 botes, y van en este orden:
// Botes del lado izquierdo: [3,2,1]
// Botes del lado derecho

// Comando GENERATE <tipo_barco> <posición>
// Genera un barco de un tipo específico en una posición específica de la cola (fila de espera).
// <tipo_barco>: Puede ser uno de los siguientes:
// PATRULLA: Barco de tipo patrulla.
// NORMAL: Barco de tipo normal.
// PESQUERO: Barco de tipo pesquero.
// <posición>: Especifica la posición en la que se generará el barco. Puede ser:
// Lado izquierdo: LEFT1, LEFT2, LEFT3.
// Lado derecho: RIGHT1, RIGHT2, RIGHT3.
// Ejemplos:
// GENERATE PATRULLA LEFT1: Genera un barco de tipo patrulla en la posición LEFT1.
// GENERATE NORMAL RIGHT2: Genera un barco de tipo normal en la posición RIGHT2.
// Si la posición especificada ya está ocupada, el Arduino responderá: La posición <posición> ya está ocupada


// Comando REMOVE <posición>
// Remueve el barco que se encuentra en la posición especificada.
// <posición>: Especifica la posición de la cual se removerá el barco. Puede ser:
// Lado izquierdo: LEFT1, LEFT2, LEFT3.
// Lado derecho: RIGHT1, RIGHT2, RIGHT3.
// Ejemplos:
// REMOVE LEFT1: Remueve el barco en la posición LEFT1.

// Si se envía un comando que no coincide con ninguno de los anteriores, el Arduino responderá: Comando desconocido

// CONSIDERACIONES
// Los comandos deben terminar con un carácter de nueva línea (\n) para que el Arduino los procese.
// Los espacios son importantes para separar los parámetros.
// El sistema no distingue entre mayúsculas y minúsculas; todos los comandos se convierten a mayúsculas internamente.

//Respuestas y Estado del Sistema
// Estado del Canal: El Arduino envía periódicamente el estado del canal y las colas por serial:
// Sentido: ->
// Cola Izq: -,P,-
// Cola Der: N,-,-
// -> indica flujo de izquierda a derecha.
// <- indica flujo de derecha a izquierda.

// Abreviaturas de los tipos de barcos en las colas:
// P: Patrulla
// N: Normal
// S: Pesquero (usamos 'S' para 'Pesquero')
// -: Posición vacía

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // Para write(), read(), sleep()
#include <fcntl.h>      // Para O_RDWR
#include <termios.h>    // Para struct termios
#include <errno.h>

#define SERIAL_PORT "/dev/cu.usbmodem1414101"  //  esto se cambia por el puerto serie donde se conecte el arduino

int main() {
    int serial_port = open(SERIAL_PORT, O_RDWR);

    // Verificar si el puerto serie se abrió correctamente
    if (serial_port < 0) {
        printf("Error %i al abrir el puerto serie: %s\n", errno, strerror(errno));
        return 1;
    }

    // Configurar las opciones del puerto serie
    struct termios tty;

    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i al obtener los atributos del puerto serie: %s\n", errno, strerror(errno));
        return 1;
    }

    // Configuración de velocidad de baudios
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Configurar 8N1 (8 bits de datos, sin paridad, 1 bit de stop)
    tty.c_cflag &= ~PARENB; // Sin bit de paridad
    tty.c_cflag &= ~CSTOPB; // 1 bit de stop
    tty.c_cflag &= ~CSIZE;  // Limpiar el tamaño de bits
    tty.c_cflag |= CS8;     // 8 bits de datos

    // Sin control de flujo
    tty.c_cflag &= ~CRTSCTS;

    // Habilitar lectura y configurar el puerto en modo local
    tty.c_cflag |= CREAD | CLOCAL;

    // Configuración de flags de entrada
    tty.c_lflag &= ~ICANON; // Modo no canónico
    tty.c_lflag &= ~ECHO;   // Sin eco
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;   // Sin interpretación de señales INTR, QUIT, SUSP

    // Configuración de flags de salida
    tty.c_oflag &= ~OPOST;  // Sin procesamiento de salida
    tty.c_oflag &= ~ONLCR;

    // Configuración de flags de entrada
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Sin control de flujo por software
    tty.c_iflag &= ~(ICRNL | INLCR);        // Sin conversión de retorno de carro

    // Bloqueo de lectura hasta que se reciba al menos 1 byte
    tty.c_cc[VTIME] = 1;  // Tiempo de espera (decisegundos)
    tty.c_cc[VMIN] = 0;   // Número mínimo de caracteres para leer

    // Aplicar configuraciones
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i al configurar el puerto serie: %s\n", errno, strerror(errno));
        return 1;
    }

    // Agregar una pausa de 2 segundos para que el Arduino se reinicie
    sleep(2);

    // Leer y descartar datos iniciales
    char initial_buf[256];
    int num_bytes = read(serial_port, &initial_buf, sizeof(initial_buf));

    printf("Conexión establecida con el Arduino en %s\n", SERIAL_PORT);
    printf("Ingresa comandos para enviar al Arduino. Escribe 'EXIT' para salir.\n");

    char command[256];
    char read_buf[256];

    while (1) {
        // Pedir al usuario que ingrese un comando
        printf(">> ");
        fflush(stdout);  // Asegurar que el prompt se muestra

        // Leer el comando desde stdin
        if (fgets(command, sizeof(command), stdin) == NULL) {
            printf("\nError al leer el comando.\n");
            break;
        }

        // Eliminar el carácter de nueva línea del comando
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
            len--;
        }

        // Si el usuario escribe "EXIT", salir del bucle
        if (strcmp(command, "EXIT") == 0) {
            printf("Saliendo...\n");
            break;
        }

        // Agregar el carácter de nueva línea que el Arduino espera
        strcat(command, "\n");

        // Enviar el comando al Arduino
        int bytes_written = write(serial_port, command, strlen(command));

        if (bytes_written < 0) {
            printf("Error al escribir en el puerto serie: %s\n", strerror(errno));
            break;
        }

        // Leer la respuesta del Arduino
        memset(&read_buf, '\0', sizeof(read_buf));

        // Esperar un breve periodo para permitir que el Arduino responda
        usleep(100000); // 100 ms

        // Leer datos disponibles del Arduino
        int total_bytes_read = 0;
        do {
            num_bytes = read(serial_port, &read_buf[total_bytes_read], sizeof(read_buf) - total_bytes_read);

            if (num_bytes < 0) {
                printf("Error al leer del puerto serie: %s\n", strerror(errno));
                break;
            }

            total_bytes_read += num_bytes;

            // Esperar un poco antes de leer de nuevo
            usleep(10000); // 10 ms
        } while (num_bytes > 0 && total_bytes_read < sizeof(read_buf) - 1);

        if (total_bytes_read > 0) {
            printf("Arduino: %s\n", read_buf);
        } else {
            printf("No se recibió respuesta del Arduino.\n");
        }
    }

    // Cerrar el puerto serie
    close(serial_port);

    return 0;
}
