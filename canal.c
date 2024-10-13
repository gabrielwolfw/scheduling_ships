#include "canal.h"
#include "calendarizacion.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

// Variables globales
int sentido_actual = 0;        // 0: izquierda a derecha, 1: derecha a izquierda
int tiempo_letrero = 10;       // Tiempo en segundos para cambiar el letrero
int longitud_canal = 10;       // Longitud por defecto del canal (puede modificarse)
int parametro_w = 3;           // Número de barcos que deben pasar de cada lado
int barcos_pasados = 0;        // Contador de barcos que han pasado en el sentido actual
ModoControlFlujo modo_actual = MODO_LETRERO;
CEmutex_t canal_mutex;         // Mutex para sincronizar el acceso al canal
CEmutex_t letrero_mutex;       // Mutex para sincronizar el cambio del letrero
SistemaCalendarizacion sistema_cal; // Sistema de calendarización
AlgoritmoCalendarizacion algoritmo_actual = FCFS; // Algoritmo de calendarización actual
bool canal_activo = true;      // Estado del canal
bool canal_ocupado = false;    // Estado del canal


int barcos_cruzados[6] = {9,9,9,9,9,9};
int indice_cruce = 0;

void registrar_cruce(int id){
    barcos_cruzados[indice_cruce] = id;
    indice_cruce++;
}

int* obtener_cruzados(){
    printf("Tamano array: %d", sizeof(barcos_cruzados)/sizeof(barcos_cruzados[0]));
    return barcos_cruzados;
}

void iniciar_canal(int tiempo_letrero_definido, int longitud_definida, ModoControlFlujo modo, int parametro_w_definido, AlgoritmoCalendarizacion algoritmo) {
    sentido_actual = 0;  // Comienza de izquierda a derecha
    tiempo_letrero = tiempo_letrero_definido;
    longitud_canal = longitud_definida;
    modo_actual = modo;
    parametro_w = parametro_w_definido;
    barcos_pasados = 0;
    algoritmo_actual = algoritmo;
    CEmutex_init(&canal_mutex);
    CEmutex_init(&letrero_mutex);
    inicializar_sistema(&sistema_cal);
}

void* cruzar_canal_letrero(void* arg) {
    Barco* barco = (Barco*) arg;

    while (barco->tiempo_restante > 0) {
        CEmutex_lock(&canal_mutex);

        // Espera hasta que sea la dirección correcta para cruzar
        while (barco->direccion != sentido_actual) {
            CEmutex_unlock(&canal_mutex);
            CEthread_sleep(1);
            CEmutex_lock(&canal_mutex);
        }

        // Obtener el siguiente barco basado en el algoritmo de calendarización
        Barco* siguiente = obtener_siguiente_barco(&sistema_cal, sentido_actual, algoritmo_actual);

        if (siguiente != barco) {
            // Si no es su turno, liberar el mutex y dormir un momento
            CEmutex_unlock(&canal_mutex);
            CEthread_sleep(1);
            continue;
        }

        // Calcular el tiempo de cruce basado en la velocidad del barco
        int tiempo_cruce = (int)(longitud_canal / barco->velocidad);
        int tiempo_this_turn = (algoritmo_actual == ROUND_ROBIN) ? 
                               (tiempo_cruce > sistema_cal.quantum ? sistema_cal.quantum : tiempo_cruce) : 
                               tiempo_cruce;

        printf("Barco %d está cruzando de %s a %s por %d segundos\n",
               barco->id,
               barco->direccion == 0 ? "izquierda" : "derecha",
               barco->direccion == 0 ? "derecha" : "izquierda",
               tiempo_this_turn);

        // Marcar el canal como ocupado
        canal_ocupado = true;
        CEmutex_unlock(&canal_mutex);

        // Simular el cruce del barco
        CEthread_sleep(tiempo_this_turn);

        CEmutex_lock(&canal_mutex);
        barco->tiempo_restante -= tiempo_this_turn;

        if (barco->tiempo_restante <= 0) {
            canal_ocupado = false;
            printf("Barco %d ha cruzado completamente.\n", barco->id);
        } else if (algoritmo_actual == ROUND_ROBIN) {
            canal_ocupado = false;
            printf("Barco %d ha usado su quantum y vuelve a la cola.\n", barco->id);
        }

        CEmutex_unlock(&canal_mutex);
    }

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

    Barco* siguiente = obtener_siguiente_barco(&sistema_cal, sentido_actual, algoritmo_actual);
    if (siguiente != barco) {
        agregar_a_cola(&sistema_cal, barco);  // Aquí es clave que esta función ya usa listas
        CEmutex_unlock(&canal_mutex);
        return NULL;
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
    sentido_actual = barco->direccion;
    CEthread_sleep(0.5);
    Barco* siguiente = obtener_siguiente_barco(&sistema_cal, sentido_actual, algoritmo_actual);
    if (siguiente != barco) {
        agregar_a_cola(&sistema_cal, barco);
        CEmutex_unlock(&canal_mutex);
        return NULL;
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

void* cambiar_sentido(void* arg) {
     (void)arg;
    while (canal_activo) {
        CEthread_sleep(tiempo_letrero);

        CEmutex_lock(&canal_mutex);
        if (!canal_ocupado) {
            sentido_actual = 1 - sentido_actual;
            printf("Cambiando el sentido del canal a %s.\n",
                   sentido_actual == 0 ? "izquierda a derecha" : "derecha a izquierda");
        }
        CEmutex_unlock(&canal_mutex);
    }
    return NULL;
}

//Agregar barco a la cola del algoritmo
void agregar_barco_al_canal(Barco* barco) {
    agregar_a_cola(&sistema_cal, barco);  
}