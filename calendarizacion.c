#include "calendarizacion.h"
#include <stdio.h>
#include <limits.h>
// Inicializar el sistema de calendarización
void inicializar_sistema(SistemaCalendarizacion *sistema) {
    sistema->izquierda.count = 0;
    sistema->derecha.count = 0;
    sistema->quantum = 5; // Valor por defecto para Round Robin
}

// Agregar un barco a la cola correspondiente
void agregar_a_cola(SistemaCalendarizacion *sistema, Barco *barco) {
    ColaBarcos *cola = (barco->direccion == 0) ? &sistema->izquierda : &sistema->derecha;
    if (cola->count < MAX_BARCOS) {
        cola->barcos[cola->count++] = barco;
    } else {
        printf("Cola de barcos %s llena.\n", (barco->direccion == 0) ? "izquierda" : "derecha");
    }
}

// FCFS (First-Come, First-Served)
Barco *obtener_siguiente_barco_fcfs(SistemaCalendarizacion *sistema, int direccion_actual) {
    ColaBarcos *cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (cola->count == 0) return NULL;

    Barco *siguiente = cola->barcos[0];
    for (int i = 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}

// Round Robin
Barco *obtener_siguiente_barco_rr(SistemaCalendarizacion *sistema, int direccion_actual) {
    ColaBarcos *cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (cola->count == 0) return NULL;

    Barco *siguiente = cola->barcos[0];
    for (int i = 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    
    if (siguiente->tiempo_restante > sistema->quantum) {
        siguiente->tiempo_restante -= sistema->quantum;
        cola->barcos[cola->count - 1] = siguiente;
    } else {
        cola->count--;
    }
    return siguiente;
}

// Tiempo Real
Barco *obtener_siguiente_barco_tiempo_real(SistemaCalendarizacion *sistema, int direccion_actual) {
    ColaBarcos *cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (cola->count == 0) return NULL;

    int index_menor_deadline = 0;
    for (int i = 1; i < cola->count; i++) {
        if (cola->barcos[i]->deadline < cola->barcos[index_menor_deadline]->deadline)
            index_menor_deadline = i;
    }

    Barco *siguiente = cola->barcos[index_menor_deadline];
    for (int i = index_menor_deadline + 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}

// Prioridad
Barco *obtener_siguiente_barco_prioridad(SistemaCalendarizacion *sistema, int direccion_actual) {
    ColaBarcos *cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (cola->count == 0) return NULL;

    int index_mayor_prioridad = 0;
    for (int i = 1; i < cola->count; i++) {
        if (cola->barcos[i]->tipo > cola->barcos[index_mayor_prioridad]->tipo)
            index_mayor_prioridad = i;
    }

    Barco *siguiente = cola->barcos[index_mayor_prioridad];
    for (int i = index_mayor_prioridad + 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}

// SJF (Shortest Job First)
Barco *obtener_siguiente_barco_sjf(SistemaCalendarizacion *sistema, int direccion_actual) {
    ColaBarcos *cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (cola->count == 0) return NULL;

    int index_menor_tiempo = 0;
    for (int i = 1; i < cola->count; i++) {
        if (cola->barcos[i]->velocidad > cola->barcos[index_menor_tiempo]->velocidad)
            index_menor_tiempo = i; // Velocidad más alta significa menor tiempo
    }

    Barco *siguiente = cola->barcos[index_menor_tiempo];
    for (int i = index_menor_tiempo + 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}

// Función general para obtener el siguiente barco según el algoritmo de calendarización
Barco *obtener_siguiente_barco(SistemaCalendarizacion *sistema, int direccion_actual, AlgoritmoCalendarizacion algoritmo) {
    switch (algoritmo) {
        case FCFS:
            return obtener_siguiente_barco_fcfs(sistema, direccion_actual);
        case ROUND_ROBIN:
            return obtener_siguiente_barco_rr(sistema, direccion_actual);
        case TIEMPO_REAL:
            return obtener_siguiente_barco_tiempo_real(sistema, direccion_actual);
        case PRIORIDAD:
            return obtener_siguiente_barco_prioridad(sistema, direccion_actual);
        case SJF:
            return obtener_siguiente_barco_sjf(sistema, direccion_actual);
        default:
            printf("Algoritmo de calendarización no reconocido\n");
            return NULL;
    }
}