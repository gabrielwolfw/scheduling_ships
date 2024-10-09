#include "canal.h"
#include "barco.h"
#include <stdio.h>

#define NUM_BARCOS 3  // Número de barcos para la prueba

int main() {
    // Configuración inicial del canal
    int tiempo_letrero = 10;     // Tiempo en segundos para cambiar el letrero
    int longitud_canal = 10;     // Longitud del canal en unidades
    int parametro_w = 3;         // Número de barcos por dirección en modo equidad (no utilizado en letrero)
    AlgoritmoCalendarizacion algoritmo = FCFS;  // Algoritmo de calendarización (FCFS en este caso)
    ModoControlFlujo modo = MODO_LETRERO;       // Modo de flujo (letrero)

    // Inicialización del canal
    iniciar_canal(tiempo_letrero, longitud_canal, modo, parametro_w, algoritmo);

    // Crear barcos
    Barco barcos[NUM_BARCOS];
    printf("Agregando barcos a la cola...\n");

    // Barco 0: derecha a izquierda, tipo Normal
    agregar_barco(barcos, 0, 1, NORMAL, longitud_canal);
    mostrar_info_barco(&barcos[0]);
    agregar_barco_al_canal(&barcos[0]);

    // Barco 1: derecha a izquierda, tipo Pesquero
    agregar_barco(barcos, 1, 1, PESQUERO, longitud_canal);
    mostrar_info_barco(&barcos[1]);
    agregar_barco_al_canal(&barcos[1]);

    // Barco 2: izquierda a derecha, tipo Patrulla
    agregar_barco(barcos, 2, 0, PATRULLA, longitud_canal);
    mostrar_info_barco(&barcos[2]);
    agregar_barco_al_canal(&barcos[2]);

    // Crear el hilo para el cambio de sentido del letrero
    CEthread_t hilo_cambio_sentido;
    if (modo == MODO_LETRERO) {
        CEthread_create(&hilo_cambio_sentido, (void*)cambiar_sentido, NULL);
    }

    // Procesar el cruce de los barcos en el canal
    CEthread_t hilos_barcos[NUM_BARCOS];
    for (int i = 0; i < NUM_BARCOS; i++) {
        CEthread_create(&hilos_barcos[i], cruzar_canal, &barcos[i]);
    }

    // Esperar a que todos los barcos crucen
    for (int i = 0; i < NUM_BARCOS; i++) {
        CEthread_join(&hilos_barcos[i], NULL);
    }

    // Detener el hilo de cambio de sentido si está activo
    if (modo == MODO_LETRERO) {
        CEthread_join(&hilo_cambio_sentido, NULL);
    }

    return 0;
}
