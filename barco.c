#include "barco.h"
#include "canal.h"  // Aquí ya se incluye la declaración de cambiar_sentido()
#include "calendarizacion.h"
#include "CEThreads.h"
#include <stdio.h>
#include <stdlib.h>

int canal_libre = 1;  // Variable para indicar si el canal está libre

// Función para agregar un barco con prioridad
void agregar_a_cola_prioridad(ColaBarcos *cola, Barco *barco) {
    if (cola->count >= MAX_BARCOS) {
        printf("Cola de barcos llena.\n");
        return;
    }

    // Insertar barcos patrulleros al inicio
    if (barco->tipo == PATRULLA) {
        for (int i = cola->count; i > 0; i--) {
            cola->barcos[i] = cola->barcos[i - 1];
        }
        cola->barcos[0] = barco;
    } else {
        // Insertar barcos pesqueros y normales al final
        cola->barcos[cola->count] = barco;
    }

    cola->count++;
}

// Función para agregar un barco
void agregar_barco(Barco* barcos, int id, int direccion, TipoBarco tipo) {
    barcos[id].id = id;
    barcos[id].direccion = direccion;

    // Ajustar la velocidad según el tipo de barco
    switch (tipo) {
        case NORMAL:
            barcos[id].velocidad = 2.0; // Ejemplo: 2 unidades/segundo para barco normal
            barcos[id].tipo = NORMAL;  // Asignar el tipo de barco
            break;
        case PESQUERO:
            barcos[id].velocidad = 1.5; // Ejemplo: 1.5 unidades/segundo para barco pesquero
            barcos[id].tipo = PESQUERO; // Asignar el tipo de barco
            break;
        case PATRULLA:
            barcos[id].velocidad = 1.0; // Ejemplo: 1 unidad/segundo para barco patrullero
            barcos[id].tipo = PATRULLA; // Asignar el tipo de barco
            break;
        default:
            barcos[id].velocidad = 2.0; // Valor por defecto
            barcos[id].tipo = NORMAL;  // Asignar tipo por defecto
            break;
    }
}

// Función para mostrar información del barco
void mostrar_info_barco(const Barco* barco) {
    const char* direccion_str = barco->direccion == 0 ? "izquierda a derecha" : "derecha a izquierda";
    const char* tipo_str;

    switch (barco->tipo) {
        case NORMAL:
            tipo_str = "Normal";
            break;
        case PESQUERO:
            tipo_str = "Pesquero";
            break;
        case PATRULLA:
            tipo_str = "Patrulla";
            break;
        default:
            tipo_str = "Desconocido";
            break;
    }

    printf("Barco ID: %d, Tipo: %s, Dirección: %s, Velocidad: %.2f unidades/segundo\n",
           barco->id, tipo_str, direccion_str, barco->velocidad);
}

// Función que simula el comportamiento del barco
void* comportamiento_barco(void* arg) {
    Barco* barco = (Barco*)arg;

    mostrar_info_barco(barco); // Mostrar información sobre el barco antes de cruzar

    // Sincronización para el cruce del canal
    CEmutex_lock(&canal_mutex);
    
    // Asegurarse de que el barco solo cruce cuando el sentido es correcto
    while (!canal_libre || barco->direccion != sentido_actual) {
        printf("Barco %d está esperando para cruzar el canal...\n", barco->id);
        CEmutex_unlock(&canal_mutex);
        CEthread_sleep(1);  // Esperar un segundo antes de volver a intentar
        CEmutex_lock(&canal_mutex);
    }

    canal_libre = 0; // Marcar el canal como ocupado
    CEmutex_unlock(&canal_mutex);

    // Simular el cruce del canal de acuerdo a la velocidad
    double tiempo_total_cruce = longitud_canal / barco->velocidad;
    printf("Barco %d (Tipo: %d) está cruzando el canal, tomará %.2f segundos.\n",
           barco->id, barco->tipo, tiempo_total_cruce);

    CEthread_sleep((int)tiempo_total_cruce); // Simular el tiempo de cruce

    printf("Barco %d ha terminado de cruzar el canal.\n", barco->id);

    // Liberar el canal
    CEmutex_lock(&canal_mutex);
    canal_libre = 1; // Marcar el canal como libre
    CEmutex_unlock(&canal_mutex);

    return NULL;
}


// Función principal que gestiona los barcos y la cola
void main_barco() {
    ColaBarcos cola;  // Crear la cola de barcos
    inicializar_cola(&cola);  // Inicializar la cola de barcos

    Barco barcos[MAX_BARCOS];  // Array para almacenar barcos
    int contador_barcos = 0;   // Contador de barcos generados
    CEthread_t hilos[MAX_BARCOS];  // Crear hilos para cada barco

    // Inicializar el canal con el modo de control de flujo
    iniciar_canal(5, 10, MODO_LETRERO, 3);  // Longitud del canal es de 10 unidades, tiempo del letrero es de 5 segundos
    printf("Canal iniciado con longitud de %d unidades y cambio de letrero cada %d segundos.\n", longitud_canal, tiempo_letrero);

    // Fase de entrada de barcos por teclado
    char opcion;
    while (1) {
        printf("Ingrese el tipo de barco a generar (n: Normal, p: Pesquero, t: Patrulla, q: Salir y procesar): ");
        scanf(" %c", &opcion);

        if (opcion == 'q') {
            break; // Salir del bucle y proceder a procesar los barcos
        }

        if (contador_barcos >= MAX_BARCOS) {
            printf("Número máximo de barcos alcanzado.\n");
            break;
        }

        TipoBarco tipo;
        int direccion;

        printf("Seleccione el océano de origen (0: Izquierda, 1: Derecha): ");
        int oceano;
        scanf(" %d", &oceano);

        if (oceano == 0) {
            direccion = 0; // Izquierda a derecha
        } else if (oceano == 1) {
            direccion = 1; // Derecha a izquierda
        } else {
            printf("Opción de océano no válida.\n");
            continue;
        }

        // Definir el tipo de barco según la entrada
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

        // Agregar barco a la cola con prioridad
        agregar_barco(barcos, contador_barcos, direccion, tipo);
        agregar_a_cola_prioridad(&cola, &barcos[contador_barcos]);
        printf("Barco %d agregado: dirección %s, tipo %s.\n", contador_barcos,
               direccion == 0 ? "izquierda a derecha" : "derecha a izquierda",
               tipo == NORMAL ? "NORMAL" : tipo == PESQUERO ? "PESQUERO" : "PATRULLA");

        contador_barcos++;  // Incrementar el contador de barcos
    }

    // Crear el hilo para manejar el cambio de sentido del letrero
    printf("Iniciando el cambio de letrero en un hilo separado...\n");
    CEthread_t hilo_letrero;
    CEthread_create(&hilo_letrero, (void*)cambiar_sentido, NULL);

    // Procesar los barcos que han sido agregados a la cola
    while (cola.count > 0) {
        // Obtener el siguiente barco según el algoritmo de calendarización
        Barco* siguiente_barco = obtener_siguiente_barco_rr(&cola);
        if (siguiente_barco) {
            printf("El siguiente barco en cruzar es el Barco %d (Dirección: %s).\n",
                   siguiente_barco->id,
                   siguiente_barco->direccion == 0 ? "izquierda a derecha" : "derecha a izquierda");

            // Crear un hilo para cruzar el canal
            CEthread_create(&hilos[siguiente_barco->id], comportamiento_barco, (void*) siguiente_barco);
            CEthread_join(&hilos[siguiente_barco->id], NULL);  // Esperar a que el barco cruce
        }
    }

    // Finalizar el hilo del letrero cuando no haya más barcos
    printf("No hay más barcos en la cola, finalizando hilo del letrero...\n");
    CEthread_end(&hilo_letrero);
}
