#include "canal.h"
#include <stdio.h>
#include <unistd.h>

// Definir variables globales
int sentido_actual = 0;        // 0: izquierda a derecha, 1: derecha a izquierda
int tiempo_letrero = 5;        // Tiempo en segundos para cambiar el letrero
CEmutex_t canal_mutex;         // Mutex para sincronizar el acceso al canal
CEmutex_t letrero_mutex;       // Mutex para sincronizar el cambio del letrero

// Inicializar el canal y las variables
void iniciar_canal(int tiempo_letrero_definido) {
    sentido_actual = 0;  // Comienza de izquierda a derecha
    tiempo_letrero = tiempo_letrero_definido;
    CEmutex_init(&canal_mutex);
    CEmutex_init(&letrero_mutex);
}

// Función para agregar un barco
void agregar_barco(Barco* barcos, int id, int direccion, int tiempo_paso) {
    barcos[id].id = id;
    barcos[id].direccion = direccion;
    barcos[id].tiempo_paso = tiempo_paso;
}

// Función que simula el cruce del barco
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

    // Simula el cruce del barco
    printf("Barco %d está cruzando de %s a %s (tomará %d segundos)\n",
           barco->id,
           barco->direccion == 0 ? "izquierda" : "derecha",
           barco->direccion == 0 ? "derecha" : "izquierda",
           barco->tiempo_paso);

    CEthread_sleep(barco->tiempo_paso);  // Simula el tiempo de cruce
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
