#include "canal.h"
#include "barco.h"
#include "calendarizacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_BARCOS 6  // Número de barcos para la prueba

void crear_barco(Barco* barcos, int id, int direccion, TipoBarco tipo, int longitud_canal) {
    inicializar_barco(&barcos[id], id, direccion, tipo, longitud_canal);
    mostrar_info_barco(&barcos[id]);
    agregar_barco_al_canal(&barcos[id]);
}

int main() {
    // Inicializar el sistema
    int tiempo_letrero = 20;  // 20 segundos para cambiar el letrero
    int longitud_canal = 10; // 100 unidades de longitud
    int parametro_w = 3;      // 3 barcos pasan antes de cambiar dirección en modo equidad
    ModoControlFlujo modo = MODO_LETRERO; // Usar modo letrero para esta prueba
    AlgoritmoCalendarizacion algoritmo = FCFS; // Usar Round Robin para esta prueba

    iniciar_canal(tiempo_letrero, longitud_canal, modo, parametro_w, algoritmo);

    // Crear arreglo de barcos
    Barco barcos[NUM_BARCOS];

    // Crear barcos con diferentes características
    crear_barco(barcos, 0, 0, NORMAL, longitud_canal);    // Izquierda a derecha, normal
    crear_barco(barcos, 1, 1, PESQUERO, longitud_canal);  // Derecha a izquierda, pesquero
    crear_barco(barcos, 2, 0, PATRULLA, longitud_canal);  // Izquierda a derecha, patrulla
    crear_barco(barcos, 3, 1, NORMAL, longitud_canal);    // Derecha a izquierda, normal
    crear_barco(barcos, 4, 0, PESQUERO, longitud_canal);  // Izquierda a derecha, pesquero
    crear_barco(barcos, 5, 1, PATRULLA, longitud_canal);  // Derecha a izquierda, patrulla

    // Crear hilos para cada barco
    CEthread_t hilos_barcos[NUM_BARCOS];
    for (int i = 0; i < NUM_BARCOS; i++) {
        CEthread_create(&hilos_barcos[i],cruzar_canal, &barcos[i]);
    }

    // Crear hilo para cambiar el sentido del canal (solo para modo letrero)
    CEthread_t hilo_cambio_sentido;
    if (modo == MODO_LETRERO) {
        CEthread_create(&hilo_cambio_sentido, (void*)cambiar_sentido, NULL);
    }

    // Esperar a que todos los barcos terminen
    for (int i = 0; i < NUM_BARCOS; i++) {
        CEthread_join(&hilos_barcos[i], NULL);
    }

    // Detener el hilo de cambio de sentido
    canal_activo = false;
    if (modo == MODO_LETRERO) {
        CEthread_join(&hilo_cambio_sentido, NULL);
    }

    // Limpiar recursos
    CEmutex_destroy(&canal_mutex);
    CEmutex_destroy(&letrero_mutex);

    printf("Simulación completa.\n");

    return 0;
}