// ESTE SCRIPT MUESTRA COMO ENVIAR UN COMANDO UNICO E INDIVIDUAL SERIALMENTE

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
/*#include "canal.h"
#include "barco.h"
#include "calendarizacion.h"
#include "CEThreads.h"
#include <stdio.h>*/

#define SERIAL_PORT "/dev/cu.usbmodem1414101"  // esto se cambia por el puerto serie donde se conecte el arduino

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
/*
     ColaBarcos cola;  // Crear la cola de barcos
    inicializar_cola(&cola);  // Inicializar la cola de barcos

    Barco barcos[MAX_BARCOS];  // Array para almacenar barcos
    int contador_barcos = 0;   // Contador de barcos generados
    CEthread_t hilos[MAX_BARCOS];  // Crear hilos para cada barco

    // Inicializar el canal con el modo de control de flujo
    iniciar_canal(5, 10, MODO_LETRERO, 3);  // Longitud del canal es de 10 unidades, tiempo del letrero es de 5 segundos
    printf("Canal iniciado con longitud de %d unidades y cambio de letrero cada %d segundos.\n", longitud_canal, tiempo_letrero);

    // Agregar barcos a la cola con diferentes configuraciones
    printf("Agregando barcos a la cola...\n");
    agregar_barco(barcos, contador_barcos++, 0, NORMAL);  // Barco 0, izquierda a derecha, normal
    agregar_a_cola(&cola, &barcos[0]);

    printf("Barco 0 agregado: dirección izquierda a derecha, tipo NORMAL.\n");*/
    char command[] = "GENERATE NORMAL LEFT1\n";
    char command2[] = "GENERATE NORMAL RIGHT2\n";
    char command3[] = "MOTOR LEFT\n";
    // Leer y descartar datos iniciales
    char initial_buf[256];
    int num_bytes = read(serial_port, &initial_buf, sizeof(initial_buf));

    // Enviar un comando al Arduino
    
    write(serial_port, command, strlen(command));
    usleep(100000);
    write(serial_port, command2, strlen(command2));
    usleep(100000);
    write(serial_port, command3, strlen(command3));
    printf("Comando enviado: %s", command);

    // Leer la respuesta del Arduino
    char read_buf[256];
    memset(&read_buf, '\0', sizeof(read_buf));

    while (1) {
        int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

        if (num_bytes < 0) {
            printf("Error al leer del puerto serie: %s\n", strerror(errno));
            return 1;
        } else if (num_bytes == 0) {
            // No hay datos disponibles, delay para esperar
            usleep(100000); // 100 ms
        } else {
            printf("Respuesta recibida (%d bytes): %s\n", num_bytes, read_buf);
            memset(&read_buf, '\0', sizeof(read_buf)); // Limpiar el buffer

            // Aca se puede agregar una condición para salir del bucle si es necesario
            // break;
        }

        // Para evitar un bucle infinito en este ejemplo, salimos después de leer la respuesta
        
        break;
    }

    // Cerrar el puerto serie
    close(serial_port);

    return 0;
}
