#include "canal.h"
#include "barco.h"
#include "CEThreads.h"
#include <stdio.h>
#include "configuracion.h"
#define NUM_BARCOS 6  // Número de barcos para la prueba

void crear_barco(Barco* barcos, int id, int direccion, TipoBarco tipo, int longitud_canal, ConfiguracionCanal *config) {
    // Inicializar el barco con los valores según el tipo
    inicializar_barco(&barcos[id], id, direccion, tipo, longitud_canal);

    // Asignar la velocidad según el tipo de barco
    switch (tipo) {
        case NORMAL:
            barcos[id].velocidad = config->velocidad_normal;
            break;
        case PESQUERO:
            barcos[id].velocidad = config->velocidad_pesquero;
            break;
        case PATRULLA:
            barcos[id].velocidad = config->velocidad_patrulla;
            break;
        default:
            printf("Tipo de barco desconocido\n");
            break;
    }

    mostrar_info_barco(&barcos[id]);
    agregar_barco_al_canal(&barcos[id]);
}


void agregar_barcos_por_teclado(Barco* barcos, int* contador_barcos, int longitud_canal, ConfiguracionCanal *config) {
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

        // Asignar el tipo de barco según la entrada del usuario
        switch (opcion) {
            case 'n':
                tipo = NORMAL;
                break;
            case 'p':
                tipo = PESQUERO;
                break;
            case 't':
                tipo = PATRULLA;
                break;
            default:
                printf("Opción no válida.\n");
                continue;
        }

        // Crear el barco y agregarlo al sistema
        crear_barco(barcos, *contador_barcos, direccion, tipo, longitud_canal, config);
        (*contador_barcos)++;  // Incrementar el contador de barcos
    }
}

int main() {
    ConfiguracionCanal config;
    leer_configuracion("config_canal.txt", &config);  // Leer el archivo de configuración

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

    printf("\nAgregando barcos a la cola...\n");
    agregar_barcos_por_teclado(barcos, &contador_barcos, config.longitud_canal, &config);

    // Crear el hilo para el cambio de sentido del letrero si aplica
    CEthread_t hilo_cambio_sentido;
    if (modo == MODO_LETRERO) {
        canal_activo = true;
        if (CEthread_create(&hilo_cambio_sentido, (void*)cambiar_sentido, NULL, -1) != 0) {
            fprintf(stderr, "Error al crear hilo para cambio de sentido\n");
            return 1;
        }
    }

    // Procesar el cruce de los barcos en el canal
    CEthread_t hilos_barcos[NUM_BARCOS];
    for (int i = 0; i < NUM_BARCOS; i++) {
        if (CEthread_create(&hilos_barcos[i], cruzar_canal, &barcos[i], barcos[i].id) != 0) {
            fprintf(stderr, "Error al crear hilo para el barco %d\n", i);
            return 1;
        }
    }

    // Esperar a que todos los barcos crucen
    for (int i = 0; i < NUM_BARCOS; i++) {
        int barco_id_cruzado;
        CEthread_join(&hilos_barcos[i], &barco_id_cruzado);
        printf("El barco con ID %d ha cruzado el canal\n", barco_id_cruzado);
    }

    // Detener el hilo de cambio de sentido si está activo
    if (modo == MODO_LETRERO) {
        canal_activo = false;
        CEthread_join(&hilo_cambio_sentido, NULL);
    }

    printf("\nSimulación completada. Todos los barcos han cruzado el canal.\n");

    return 0;
}