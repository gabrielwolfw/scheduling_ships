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

// Remover un barco de la cola
void remover_de_cola(NodoBarco **cola, Barco *barco) {
    NodoBarco *actual = *cola;
    NodoBarco *anterior = NULL;

    while (actual != NULL && actual->barco != barco) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) return; // No se encontró el barco

    if (anterior == NULL) {
        *cola = actual->siguiente; // Eliminar el primer nodo
    } else {
        anterior->siguiente = actual->siguiente;
    }

    free(actual);
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

    while (actual != NULL) {
        if (actual->barco->tipo > mayorPrioridad->barco->tipo) {
            mayorPrioridad = actual;
            anteriorMayor = anterior;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    Barco *siguiente = mayorPrioridad->barco;

    // Eliminar barco seleccionado de la lista
    if (anteriorMayor == NULL) {
        *cola = mayorPrioridad->siguiente;
    } else {
        anteriorMayor->siguiente = mayorPrioridad->siguiente;
    }

    free(mayorPrioridad);
    return siguiente;
}

// Algoritmo de SJF (Shortest Job First)
Barco *obtener_siguiente_barco_sjf(SistemaCalendarizacion *sistema, int direccion_actual) {
    NodoBarco **cola = (direccion_actual == 0) ? &sistema->izquierda : &sistema->derecha;
    if (*cola == NULL) return NULL;

    NodoBarco *actual = *cola;
    NodoBarco *menorTiempo = actual;
    NodoBarco *anterior = NULL, *anteriorMenor = NULL;

    while (actual != NULL) {
        if (actual->barco->velocidad < menorTiempo->barco->velocidad) {  // Velocidad más alta significa menor tiempo
            menorTiempo = actual;
            anteriorMenor = anterior;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    Barco *siguiente = menorTiempo->barco;

    // Eliminar barco seleccionado de la lista
    if (anteriorMenor == NULL) {
        *cola = menorTiempo->siguiente;
    } else {
        anteriorMenor->siguiente = menorTiempo->siguiente;
    }

    free(menorTiempo);
    return siguiente;
}
