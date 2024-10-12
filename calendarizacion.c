#include "calendarizacion.h"
#include <stdio.h>
#include <stdlib.h>


// Inicializar el sistema de calendarización
void inicializar_sistema(SistemaCalendarizacion *sistema) {
    sistema->izquierda = NULL;
    sistema->derecha = NULL;
    sistema->quantum = 5; // Valor por defecto para Round Robin
}

// Agregar un barco a la cola correspondiente
void agregar_a_cola(SistemaCalendarizacion *sistema, Barco *barco) {
    NodoBarco **cola = (barco->direccion == 0) ? &sistema->izquierda : &sistema->derecha;
    NodoBarco *nuevoNodo = (NodoBarco *)malloc(sizeof(NodoBarco));
    nuevoNodo->barco = barco;
    nuevoNodo->siguiente = NULL;

    if (*cola == NULL) {
        *cola = nuevoNodo;
    } else {
        NodoBarco *actual = *cola;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
}

// Función general para obtener el siguiente barco según el algoritmo seleccionado
Barco *obtener_siguiente_barco(SistemaCalendarizacion *sistema, int direccion_actual, AlgoritmoCalendarizacion algoritmo) {
    switch (algoritmo) {
        case FCFS:
            return obtener_siguiente_barco_fcfs(sistema, direccion_actual);
        case ROUND_ROBIN:
            return obtener_siguiente_barco_rr(sistema, direccion_actual);
        case PRIORIDAD:
            return obtener_siguiente_barco_prioridad(sistema, direccion_actual);
        case SJF:
            return obtener_siguiente_barco_sjf(sistema, direccion_actual);
        case TIEMPO_REAL:
            return obtener_siguiente_barco_tiempo_real(sistema, direccion_actual);
        default:
            return NULL; // Algoritmo desconocido
    }
}

// FCFS (First-Come, First-Served)
Barco *obtener_siguiente_barco_fcfs(SistemaCalendarizacion *sistema, int direccion_actual) {
    NodoBarco **cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (*cola == NULL) return NULL;

    Barco *siguiente = (*cola)->barco;
    NodoBarco *nodoAEliminar = *cola;
    *cola = (*cola)->siguiente;
    free(nodoAEliminar);
    return siguiente;
}

// Round Robin
Barco *obtener_siguiente_barco_rr(SistemaCalendarizacion *sistema, int direccion_actual) {
    NodoBarco **cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (*cola == NULL) return NULL;

    Barco *siguiente = (*cola)->barco;

    // Mover el primer nodo al final (rotar)
    if ((*cola)->siguiente != NULL) {
        NodoBarco *primerNodo = *cola;
        *cola = (*cola)->siguiente;
        NodoBarco *ultimo = *cola;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = primerNodo;
        primerNodo->siguiente = NULL;
    }

    return siguiente;
}

// Algoritmo de Prioridad
Barco *obtener_siguiente_barco_prioridad(SistemaCalendarizacion *sistema, int direccion_actual) {
    NodoBarco **cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (*cola == NULL) return NULL;

    NodoBarco *actual = *cola;
    NodoBarco *mayorPrioridad = actual;
    NodoBarco *anterior = NULL, *anteriorMayor = NULL;

    // Recorrer la lista buscando el barco con la mayor prioridad (mayor número)
    while (actual != NULL) {
        if (actual->barco->prioridad > mayorPrioridad->barco->prioridad) {  // Comparar usando la prioridad
            mayorPrioridad = actual;
            anteriorMayor = anterior;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    Barco *siguiente = mayorPrioridad->barco;

    // Eliminar barco seleccionado de la lista
    if (anteriorMayor == NULL) {
        *cola = mayorPrioridad->siguiente;  // Si es el primero en la lista
    } else {
        anteriorMayor->siguiente = mayorPrioridad->siguiente;  // Saltar el nodo mayorPrioridad
    }

    free(mayorPrioridad);  // Liberar la memoria del nodo
    return siguiente;
}

// Algoritmo de SJF (Shortest Job First)
Barco *obtener_siguiente_barco_sjf(SistemaCalendarizacion *sistema, int direccion_actual) {
    NodoBarco **cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (*cola == NULL) return NULL;  // Si no hay barcos en la cola

    NodoBarco *actual = *cola;
    NodoBarco *menorTiempo = actual;
    NodoBarco *anterior = NULL, *anteriorMenor = NULL;

    while (actual != NULL) {
        // Comparar el tiempo restante, no la velocidad
        if (actual->barco->tiempo_restante < menorTiempo->barco->tiempo_restante) {
            menorTiempo = actual;  // Actualizar el nodo con menor tiempo restante
            anteriorMenor = anterior;  // Guardar el nodo anterior
        }
        anterior = actual;  // Mover al siguiente nodo
        actual = actual->siguiente;
    }

    Barco *siguiente = menorTiempo->barco;

    // Eliminar barco seleccionado de la lista
    if (anteriorMenor == NULL) {
        *cola = menorTiempo->siguiente;  // Si el barco seleccionado es el primero
    } else {
        anteriorMenor->siguiente = menorTiempo->siguiente;  // Enlazar el anterior con el siguiente
    }

    free(menorTiempo);  // Liberar la memoria del nodo
    return siguiente;  // Devolver el barco seleccionado
}


Barco* obtener_siguiente_barco_tiempo_real(SistemaCalendarizacion *sistema, int direccion_actual) {
    NodoBarco **cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (*cola == NULL) return NULL;

    NodoBarco *actual = *cola;
    NodoBarco *menorDeadline = actual;
    NodoBarco *anterior = NULL, *anteriorMenor = NULL;

    // Buscar el barco con el menor deadline
    while (actual != NULL) {
        if (actual->barco->deadline < menorDeadline->barco->deadline) {
            menorDeadline = actual;
            anteriorMenor = anterior;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    Barco *siguiente = menorDeadline->barco;

    // Eliminar barco seleccionado de la lista, pero NO liberar el barco
    if (anteriorMenor == NULL) {
        *cola = menorDeadline->siguiente;  // Eliminar el primer nodo
    } else {
        anteriorMenor->siguiente = menorDeadline->siguiente;  // Saltar el nodo menorDeadline
    }

    free(menorDeadline);  // Liberar solo el nodo, no el barco
    return siguiente;  // Retornar el barco seleccionado
}

// Actualizar si hay barcos pendientes en el sistema
bool hay_barcos_pendientes(SistemaCalendarizacion *sistema) {
    return (sistema->izquierda != NULL || sistema->derecha != NULL);
}