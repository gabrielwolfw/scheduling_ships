#ifndef CALENDARIZACION_H
#define CALENDARIZACION_H

#include "barco.h"

// Definición de un nodo para la lista enlazada de barcos
typedef struct NodoBarco {
    Barco *barco;
    struct NodoBarco *siguiente;
} NodoBarco;

// Sistema de calendarización que contiene las colas izquierda y derecha
typedef struct {
    NodoBarco *izquierda;  // Lista enlazada de barcos que se dirigen a la izquierda
    NodoBarco *derecha;    // Lista enlazada de barcos que se dirigen a la derecha
    int quantum;           // Para el algoritmo Round Robin
} SistemaCalendarizacion;

// Enumeración para los algoritmos de calendarización
typedef enum {
    FCFS,
    ROUND_ROBIN,
    TIEMPO_REAL,
    PRIORIDAD,
    SJF
} AlgoritmoCalendarizacion;

// Inicializar el sistema de calendarización
void inicializar_sistema(SistemaCalendarizacion *sistema);

// Agregar un barco a la cola correspondiente
void agregar_a_cola(SistemaCalendarizacion *sistema, Barco *barco);

// Remover un barco de la cola correspondiente
void remover_de_cola(NodoBarco **cola, Barco *barco);

// Función general para obtener el siguiente barco según el algoritmo de calendarización
Barco *obtener_siguiente_barco(SistemaCalendarizacion *sistema, int direccion_actual, AlgoritmoCalendarizacion algoritmo);

// Algoritmos de calendarización específicos (ahora son funciones internas)
Barco *obtener_siguiente_barco_fcfs(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_rr(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_tiempo_real(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_prioridad(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_sjf(SistemaCalendarizacion *sistema, int direccion_actual);

#endif // CALENDARIZACION_H
