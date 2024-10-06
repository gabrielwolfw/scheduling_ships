#include "canal.h"
#include "barco.h"
#include <stdio.h>
#include <unistd.h>

// Definir variables globales
int sentido_actual = 0;        // 0: izquierda a derecha, 1: derecha a izquierda
int tiempo_letrero = 10;       // Tiempo en segundos para cambiar el letrero
int longitud_canal = 10;       // Longitud por defecto del canal (puede modificarse)
CEmutex_t canal_mutex;         // Mutex para sincronizar el acceso al canal
CEmutex_t letrero_mutex;       // Mutex para sincronizar el cambio del letrero

// Inicializar el canal y las variables, incluyendo la longitud del canal
void iniciar_canal(int tiempo_letrero_definido, int longitud_definida) {
    sentido_actual = 0;  // Comienza de izquierda a derecha
    tiempo_letrero = tiempo_letrero_definido;
    longitud_canal = longitud_definida;  // Establecer la longitud del canal
    CEmutex_init(&canal_mutex);
    CEmutex_init(&letrero_mutex);
}

void* cruzar_canal(void* arg) {
    Barco* barco = (Barco*) arg;

    // Bloquear el canal con el mutex
    CEmutex_lock(&canal_mutex);

    // Esperar hasta que el sentido del canal coincida con la dirección del barco
    while (barco->direccion != sentido_actual) {
        // Soltar el mutex si no es el turno del barco
        CEmutex_unlock(&canal_mutex);
        CEthread_sleep(1);  // Esperar un segundo antes de verificar de nuevo
        CEmutex_lock(&canal_mutex);  // Intentar de nuevo bloquear el canal
    }

    // Simula el cruce del barco ajustando el tiempo según la longitud del canal
    int tiempo_total_cruce = barco->velocidad * longitud_canal;  // El tiempo se basa en la velocidad
    printf("Barco %d está cruzando de %s a %s (tomará %d segundos)\n",
           barco->id,
           barco->direccion == 0 ? "izquierda" : "derecha",
           barco->direccion == 0 ? "derecha" : "izquierda",
           tiempo_total_cruce);

    CEthread_sleep(tiempo_total_cruce);  // Simula el tiempo de cruce
    printf("Barco %d ha cruzado.\n", barco->id);

    // Desbloquear el canal para el siguiente barco
    CEmutex_unlock(&canal_mutex);
    return NULL;
}

// Función que cambia el sentido del letrero
void cambiar_sentido() {
    while (1) {
        CEthread_sleep(tiempo_letrero);  // Esperar el tiempo definido para cambiar el letrero

        // Bloquear el letrero con el mutex
        CEmutex_lock(&letrero_mutex);

        // Cambiar el sentido del canal
        sentido_actual = 1 - sentido_actual;
        printf("Cambiando el sentido del canal a %s.\n",
               sentido_actual == 0 ? "izquierda a derecha" : "derecha a izquierda");

        // Desbloquear el letrero
        CEmutex_unlock(&letrero_mutex);
    }
}
