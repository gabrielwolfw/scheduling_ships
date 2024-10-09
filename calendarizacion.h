#ifndef CALENDARIZACION_H
#define CALENDARIZACION_H

#include "barco.h"

#define MAX_BARCOS 100

typedef struct {
    Barco *barcos[MAX_BARCOS];
    int count;
} ColaBarcos;

typedef struct {
    ColaBarcos izquierda;
    ColaBarcos derecha;
    int quantum;  // Para Round Robin
} SistemaCalendarizacion;

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

// Función general para obtener el siguiente barco según el algoritmo de calendarización
Barco *obtener_siguiente_barco(SistemaCalendarizacion *sistema, int direccion_actual, AlgoritmoCalendarizacion algoritmo);

// Algoritmos de calendarización específicos (ahora son funciones internas)
Barco *obtener_siguiente_barco_fcfs(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_rr(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_tiempo_real(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_prioridad(SistemaCalendarizacion *sistema, int direccion_actual);
Barco *obtener_siguiente_barco_sjf(SistemaCalendarizacion *sistema, int direccion_actual);

#endif // CALENDARIZACION_H