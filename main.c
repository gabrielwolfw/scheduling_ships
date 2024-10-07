#include "canal.h"
#include "barco.h"
#include "calendarizacion.h"
#include "CEThreads.h"
#include <stdio.h>

int main() {
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
    printf("Barco 0 agregado: dirección izquierda a derecha, tipo NORMAL.\n");

    agregar_barco(barcos, contador_barcos++, 1, PESQUERO);  // Barco 1, derecha a izquierda, pesquero
    agregar_a_cola(&cola, &barcos[1]);
    printf("Barco 1 agregado: dirección derecha a izquierda, tipo PESQUERO.\n");

    agregar_barco(barcos, contador_barcos++, 0, PATRULLA);  // Barco 2, izquierda a derecha, patrulla
    agregar_a_cola(&cola, &barcos[2]);
    printf("Barco 2 agregado: dirección izquierda a derecha, tipo PATRULLA.\n");

    // Crear el hilo para manejar el cambio de sentido del letrero
    printf("Iniciando el cambio de letrero en un hilo separado...\n");
    CEthread_t hilo_letrero;
    CEthread_create(&hilo_letrero, (void*)cambiar_sentido, NULL);

    // Procesar barcos en el canal según el algoritmo de calendarización (FCFS en este caso)
    while (cola.count > 0) {
        // Obtener el siguiente barco según el algoritmo de calendarización
        Barco* siguiente_barco = obtener_siguiente_barco_rr(&cola);
        if (siguiente_barco) {
            printf("El siguiente barco en cruzar es el Barco %d (Dirección: %s).\n",
                   siguiente_barco->id,
                   siguiente_barco->direccion == 0 ? "izquierda a derecha" : "derecha a izquierda");

            // Crear un hilo para cruzar el canal
            CEthread_create(&hilos[siguiente_barco->id], cruzar_canal, (void*) siguiente_barco);
            CEthread_join(&hilos[siguiente_barco->id], NULL);  // Esperar a que el barco cruce
        }
    }

    // Finalizar el hilo del letrero cuando no haya más barcos
    printf("No hay más barcos en la cola, finalizando hilo del letrero...\n");
    CEthread_end(&hilo_letrero);

    return 0;
}
