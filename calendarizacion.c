#include "calendarizacion.h"
#include <stdio.h>

// Inicializar la cola de barcos
void inicializar_cola(ColaBarcos* cola) {
    cola->count = 0;  // Inicializa el contador de barcos
}

// Agregar un barco a la cola
void agregar_a_cola(ColaBarcos* cola, Barco* barco) {
    if (cola->count < MAX_BARCOS) {
        cola->barcos[cola->count++] = barco;  // Agregar el barco a la cola
    } else {
        printf("Cola de barcos llena.\n");
    }
}

// Obtener el siguiente barco según FCFS (First-Come, First-Served)
Barco* obtener_siguiente_barco_fcfs(ColaBarcos* cola) {
    if (cola->count == 0) {
        return NULL;  // No hay barcos en la cola
    }
    Barco* siguiente = cola->barcos[0];  // Tomar el primer barco de la cola

    // Desplazar la cola hacia adelante
    for (int i = 1; i < cola->count; i++) {
        cola->barcos[i - 1] = cola->barcos[i];
    }
    cola->count--;  // Reducir el conteo

    return siguiente;
}

// Implementaciones adicionales para otros algoritmos (ejemplo Round Robin, Prioridad, etc.)
// Por ejemplo, para Round Robin:
Barco* obtener_siguiente_barco_rr(ColaBarcos* cola) {
    // Implementar la lógica de Round Robin aquí
    return NULL;
}

Barco* obtener_siguiente_barco_prioridad(ColaBarcos* cola) {
    // Implementar la lógica de Prioridad aquí
    return NULL;
}

Barco* obtener_siguiente_barco_sjf(ColaBarcos* cola) {
    // Implementar la lógica de Shortest Job First aquí
    return NULL;
}
