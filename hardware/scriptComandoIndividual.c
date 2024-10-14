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

// Respuestas y Estado del Sistema
//  Estado del Canal: El Arduino envía periódicamente el estado del canal y las colas por serial:
//  Sentido: ->
//  Cola Izq: -,P,-
//  Cola Der: N,-,-
//  -> indica flujo de izquierda a derecha.
//  <- indica flujo de derecha a izquierda.

// Abreviaturas de los tipos de barcos en las colas:
// P: Patrulla
// N: Normal
// S: Pesquero (usamos 'S' para 'Pesquero')
// -: Posición vacía

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Para write(), read(), sleep()
#include <fcntl.h>   // Para O_RDWR
#include <termios.h> // Para struct termios
#include <errno.h>
#include "../canal.h"
#include "../barco.h"
#include "../calendarizacion.h"
#include "../CEThreads.h"
#include "../configuracion.h"
#include <stdio.h>
#include "../configuracion.h"
#define NUM_BARCOS 6  // Número de barcos para la prueba


#define SERIAL_PORT "/dev/ttyACM0" // esto se cambia por el puerto serie donde se conecte el arduino
#define NUM_BARCOS 5               // Número de barcos para la prueba
CEmutex_t sentidoMutex;
void *enviar_commando(char *comando, void *arg)
{
    int port = (int)arg;
    printf("%s \n", comando);
    write(port, comando, strlen(comando));
    usleep(1000000);
}

int leds[6] = {9, 9, 9, 9, 9, 9};

void *chequear_colas(void *arg)
{
    while (1)
    {
        int *lista_cruzados = obtener_cruzados();

        if (lista_cruzados != NULL)
        {
            for (int i = 0; i < 6; i++)
            {
                if (lista_cruzados[i] != 9)
                {
                    int id = lista_cruzados[i];
                    for(int j = 0; j < 6; j++){
                        if(leds[j]==id){
                            if(j >= 0 && j <= 2){
                                //printf("Ya cruzo: %d \n", lista_cruzados[i]);
                                char buff[14];
                                snprintf(buff, 14, "REMOVE LEFT%d", j+1);
                                enviar_commando(buff, arg);
                            }
                            else{
                                //printf("Ya cruzo: %d \n", lista_cruzados[i]);
                                char buff[14];
                                snprintf(buff, 14, "REMOVE RIGHT%d", j-2);
                                enviar_commando(buff, arg);
                            }
                        }
                    }

                }
            }
            usleep(200000);
        }
        
    }
}
void *chequearSentido(void *arg)
{
    CEmutex_init(&sentidoMutex);
    int port = (int)arg;
    while (1)
    {
        //printf("Chequeando Sentido");
        if (sentido_actual)
        {
            printf("girando izquierda\n");
            CEmutex_lock(&sentidoMutex);
            enviar_commando("MOTOR LEFT\n", port);
            CEmutex_unlock(&sentidoMutex);
        }
        else
        {
            printf("girando derecha\n");
            CEmutex_lock(&sentidoMutex);
            enviar_commando("MOTOR RIGHT\n", port);
            CEmutex_unlock(&sentidoMutex);
        }
    }
}

void crear_barco(Barco* barcos, int id, int direccion, TipoBarco tipo, int longitud_canal, int* leds, int serial_port) {
    // Inicializar el barco
    inicializar_barco(&barcos[id], id, direccion, tipo, longitud_canal);

    // Asignar la velocidad según el tipo de barco
    switch (tipo) {
        case NORMAL:
            barcos[id].velocidad = 10; // Ejemplo de velocidad
            break;
        case PESQUERO:
            barcos[id].velocidad = 8;  // Ejemplo de velocidad
            break;
        case PATRULLA:
            barcos[id].velocidad = 12; // Ejemplo de velocidad
            break;
        default:
            printf("Tipo de barco desconocido\n");
            return;
    }

    // Determinar el tipo de barco en formato de comando
    char tipo_barco[10];
    switch (tipo) {
        case NORMAL:
            strcpy(tipo_barco, "NORMAL");
            break;
        case PESQUERO:
            strcpy(tipo_barco, "PESQUERO");
            break;
        case PATRULLA:
            strcpy(tipo_barco, "PATRULLA");
            break;
    }

    // Determinar el comando y la posición del LED
    char comando[50];
    if (direccion == 0) { // Izquierda
        snprintf(comando, sizeof(comando), "GENERATE %s LEFT%d\n", tipo_barco, id + 1);
        leds[id] = id; // Asignar el ID del barco a la posición del LED correspondiente
    } else { // Derecha
        snprintf(comando, sizeof(comando), "GENERATE %s RIGHT%d\n", tipo_barco, id + 1);
        leds[id + 3] = id; // Asignar el ID del barco a la posición del LED correspondiente
    }

    // Enviar el comando GENERATE
    enviar_commando(comando, (void *)serial_port);
    printf("ID del barco %d asignado al LED %d\n", id, (direccion == 0 ? id : id + 3));
}

void agregar_barcos_por_teclado(Barco* barcos, int* contador_barcos, int longitud_canal, ConfiguracionCanal *config, int serial_port, int* leds) {
    char opcion;

    while (1) {
        printf("Ingrese el tipo de barco a generar (n: Normal, p: Pesquero, t: Patrulla, q: Salir y procesar): ");
        scanf(" %c", &opcion);

        if (opcion == 'q') {
            break; // Salir del bucle y proceder a procesar los barcos
        }

        if (*contador_barcos >= NUM_BARCOS) {
            printf("Número máximo de barcos alcanzado.\n");
            break;
        }

        TipoBarco tipo;
        int direccion;

        // Solicitar la dirección del barco
        printf("Seleccione el océano de origen (0: Izquierda, 1: Derecha): ");
        int oceano;
        scanf(" %d", &oceano);

        direccion = (oceano == 0) ? 0 : 1;  // Izquierda a derecha o derecha a izquierda
        char* tipo_valor;

        // Asignar el tipo de barco según la entrada del usuario
        switch (opcion) {
            case 'n':
                tipo = NORMAL;
                tipo_valor = "NORMAL";
                break;
            case 'p':
                tipo = PESQUERO;
                tipo_valor = "PESQUERO";
                break;
            case 't':
                tipo = PATRULLA;
                tipo_valor = "PATRULLA";
                break;
            default:
                printf("Opción no válida.\n");
                continue;
        }

        // Crear el barco y agregarlo al sistema
        crear_barco(barcos, *contador_barcos, direccion, tipo, longitud_canal, leds, serial_port);
        (*contador_barcos)++;  // Incrementar el contador de barcos
    }
}

int main()
{
    int serial_port = open(SERIAL_PORT, O_RDWR);

    // Verificar si el puerto serie se abrió correctamente
    if (serial_port < 0)
    {
        printf("Error %i al abrir el puerto serie: %s\n", errno, strerror(errno));
        return 1;
    }

    // Configurar las opciones del puerto serie
    struct termios tty;

    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i al obtener los atributos del puerto serie: %s\n", errno, strerror(errno));
        return 1;
    }

    // Configuración de velocidad de baudios
    cfsetispeed(&tty, B230400);
    cfsetospeed(&tty, B230400);

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
    tty.c_lflag &= ~ISIG; // Sin interpretación de señales INTR, QUIT, SUSP

    // Configuración de flags de salida
    tty.c_oflag &= ~OPOST; // Sin procesamiento de salida
    tty.c_oflag &= ~ONLCR;

    // Configuración de flags de entrada
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Sin control de flujo por software
    tty.c_iflag &= ~(ICRNL | INLCR);        // Sin conversión de retorno de carro

    // Bloqueo de lectura hasta que se reciba al menos 1 byte
    tty.c_cc[VTIME] = 1; // Tiempo de espera (decisegundos)
    tty.c_cc[VMIN] = 0;  // Número mínimo de caracteres para leer

    // Aplicar configuraciones
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        printf("Error %i al configurar el puerto serie: %s\n", errno, strerror(errno));
        return 1;
    }

    // Agregar una pausa de 2 segundos para que el Arduino se reinicie
    sleep(2);

    // Configuración inicial del canal
    ConfiguracionCanal config;
    leer_configuracion("../config_canal.txt", &config);  // Leer el archivo de configuración

    // Obtener el modo de control de flujo y el algoritmo de calendarización
    ModoControlFlujo modo = obtener_modo_control_flujo(config.control_flujo);
    AlgoritmoCalendarizacion algoritmo = obtener_algoritmo_calendarizacion(config.algoritmo);

    // Inicializar el canal con los parámetros del archivo de configuración
    iniciar_canal(config.tiempo_letrero, config.longitud_canal, modo, config.parametro_w, algoritmo);
    sistema_cal.quantum = config.quantum;  // Establecer el quantum para Round Robin

    // Mostrar configuración del canal
    printf("Configuración del canal:\n");
    printf("- Modo de control de flujo: %s\n", config.control_flujo);
    printf("- Longitud del canal: %d unidades\n", config.longitud_canal);
    printf("- Velocidad barco Normal: %.2f\n", config.velocidad_normal);
    printf("- Velocidad barco Pesquero: %.2f\n", config.velocidad_pesquero);
    printf("- Velocidad barco Patrulla: %.2f\n", config.velocidad_patrulla);
    printf("- Algoritmo de calendarización: %s\n", config.algoritmo);
    printf("- Quantum (si aplica): %d\n", config.quantum);

    Barco barcos[NUM_BARCOS];
    int contador_barcos = 0;  // Inicializar contador de barcos
    int leds[6]; // Asociar directamente con el ID de cada barco
    enviar_commando("MOTOR STOP\n", (void*)serial_port);
    printf("\nAgregando barcos a la cola...\n");
    agregar_barcos_por_teclado(barcos, &contador_barcos, config.longitud_canal, &config, serial_port, leds);

    

    // Crear el hilo para el cambio de sentido del letrero
    CEthread_t hilo_cambio_sentido;
    if (modo == MODO_LETRERO)
    {
        canal_activo = true;
        if (CEthread_create(&hilo_cambio_sentido, (void *)cambiar_sentido, NULL, 0) != 0)
        {
            fprintf(stderr, "Error al crear hilo para cambio de sentido\n");
            return 1;
        }
    }
    
    CEthread_t monitorear_sentido;
    CEthread_create(&monitorear_sentido, (void *)chequearSentido, (void *)serial_port, 0);

    CEthread_t monitorear_Cruce;
    CEthread_create(&monitorear_Cruce, (void *)chequear_colas, (void *)serial_port, 0);

    
    // Procesar el cruce de los barcos en el canal
    CEthread_t hilos_barcos[NUM_BARCOS];
    for (int i = 0; i < NUM_BARCOS; i++)
    {
        if (CEthread_create(&hilos_barcos[i], cruzar_canal, &barcos[i], barcos[i].id) != 0)
        {
            fprintf(stderr, "Error al crear hilo para el barco %d\n", i);
            return 1;
        }
    }
    

    // Esperar a que todos los barcos crucen
    for (int i = 0; i < NUM_BARCOS; i++)
    {
        int barco_id_cruzado;
        CEthread_join(&hilos_barcos[i], &barco_id_cruzado);
        printf("El barco con ID %d ha cruzado el canal\n", barco_id_cruzado);
    }
    
    // Detener el hilo de cambio de sentido si está activo
    if (modo == MODO_LETRERO)
    {
        canal_activo = false;
        CEthread_join(&hilo_cambio_sentido, NULL);
    }
    usleep(5000000);
    printf("\nSimulación completada. Todos los barcos han cruzado el canal.\n");
    usleep(2000000);
    enviar_commando("MOTOR STOP\n", (void*)serial_port);

    // Cerrar el puerto serie
    close(serial_port);

    return 0;
}
