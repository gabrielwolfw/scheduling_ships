#include "canal.h"
#include "barco.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

// Definir variables globales
int sentido_actual = 0;        // 0: izquierda a derecha, 1: derecha a izquierda
int tiempo_letrero = 10;       // Tiempo en segundos para cambiar el letrero
int terminar_letrero = 0;  // Indicar cuándo detener el letrero
int barcos_en_cola[2] = {0, 0};  // Barcos esperando en cada dirección
int longitud_canal = 10;       // Longitud por defecto del canal (puede modificarse)
int parametro_w = 3;           // Número de barcos que deben pasar de cada lado
int barcos_pasados = 0;        // Contador de barcos que han pasado en el sentido actual
ModoControlFlujo modo_actual = MODO_LETRERO;
CEmutex_t canal_mutex;         // Mutex para sincronizar el acceso al canal
CEmutex_t letrero_mutex;       // Mutex para sincronizar el cambio del letrero

void iniciar_canal(int tiempo_letrero_definido, int longitud_definida, ModoControlFlujo modo, int parametro_w_definido) {
    sentido_actual = 0;  // Comienza de izquierda a derecha
    tiempo_letrero = tiempo_letrero_definido;
    longitud_canal = longitud_definida;
    modo_actual = modo;
    parametro_w = parametro_w_definido;
    barcos_pasados = 0;
    terminar_letrero = 0;  // Inicializar a 0 (activo)
    canal_libre = 1;  // Inicializar el canal como libre

    CEmutex_init(&canal_mutex);
    CEmutex_init(&letrero_mutex);
}


void* cruzar_canal_letrero(void* arg) {
    Barco* barco = (Barco*) arg;

    CEmutex_lock(&canal_mutex);

    while (barco->direccion != sentido_actual) {
        CEmutex_unlock(&canal_mutex);
        CEthread_sleep(1);
        CEmutex_lock(&canal_mutex);
    }

    int tiempo_total_cruce = longitud_canal / barco->velocidad;
    printf("Barco %d está cruzando de %s a %s (tomará %d segundos)\n",
           barco->id,
           barco->direccion == 0 ? "izquierda" : "derecha",
           barco->direccion == 0 ? "derecha" : "izquierda",
           tiempo_total_cruce);

    CEthread_sleep(tiempo_total_cruce);
    printf("Barco %d ha cruzado.\n", barco->id);

    CEmutex_unlock(&canal_mutex);
    return NULL;
}

void* cruzar_canal_equidad(void* arg) {
    Barco* barco = (Barco*) arg;

    CEmutex_lock(&canal_mutex);

    while (barco->direccion != sentido_actual || barcos_pasados >= parametro_w) {
        CEmutex_unlock(&canal_mutex);
        CEthread_sleep(1);
        CEmutex_lock(&canal_mutex);
    }

    int tiempo_total_cruce = longitud_canal / barco->velocidad;

    printf("Barco %d está cruzando de %s a %s (tomará %d segundos)\n",
           barco->id,
           barco->direccion == 0 ? "izquierda" : "derecha",
           barco->direccion == 0 ? "derecha" : "izquierda",
           tiempo_total_cruce);

    CEthread_sleep(tiempo_total_cruce);
    printf("Barco %d ha cruzado.\n", barco->id);

    barcos_pasados++;
    if (barcos_pasados >= parametro_w) {
        sentido_actual = 1 - sentido_actual;
        barcos_pasados = 0;
        printf("Cambiando el sentido del canal a %s.\n",
               sentido_actual == 0 ? "izquierda a derecha" : "derecha a izquierda");
    }

    CEmutex_unlock(&canal_mutex);
    return NULL;
}

void* cruzar_canal_tico(void* arg) {
    Barco* barco = (Barco*) arg;

    CEmutex_lock(&canal_mutex);

    int tiempo_total_cruce = longitud_canal / barco->velocidad;

    printf("Barco %d está cruzando de %s a %s (tomará %d segundos)\n",
           barco->id,
           barco->direccion == 0 ? "izquierda" : "derecha",
           barco->direccion == 0 ? "derecha" : "izquierda",
           tiempo_total_cruce);

    CEthread_sleep(tiempo_total_cruce);
    printf("Barco %d ha cruzado.\n", barco->id);

    CEmutex_unlock(&canal_mutex);
    return NULL;
}

void* cruzar_canal(void* arg) {
    switch (modo_actual) {
        case MODO_LETRERO:
            return cruzar_canal_letrero(arg);
        case MODO_EQUIDAD:
            return cruzar_canal_equidad(arg);
        case MODO_TICO:
            return cruzar_canal_tico(arg);
        default:
            printf("Modo de control de flujo no reconocido\n");
            return NULL;
    }
}

void cambiar_sentido() {
    while (!terminar_letrero) {
        CEthread_sleep(tiempo_letrero);

        CEmutex_lock(&letrero_mutex);

        // No cambiar el sentido si hay un barco cruzando actualmente
        CEmutex_lock(&canal_mutex); // Asegura acceso exclusivo al estado del canal
        if (canal_libre) {
            // Solo cambia el sentido si no hay barcos cruzando
            if (modo_actual == MODO_LETRERO) {
                sentido_actual = 1 - sentido_actual;
                printf("Cambiando el sentido del canal a %s.\n",
                       sentido_actual == 0 ? "izquierda a derecha" : "derecha a izquierda");
            }
        }
        CEmutex_unlock(&canal_mutex);

        CEmutex_unlock(&letrero_mutex);
    }
}
