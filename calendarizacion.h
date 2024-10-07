#ifndef CALENDARIZACION_H
#define CALENDARIZACION_H

#include "barco.h"

#define MAX_BARCOS 100

typedef struct {
    Barco *barcos[MAX_BARCOS];
    int count;
    int quantum;  // Para Round Robin
} ColaBarcos;

// Inicializar la cola de barcos
void inicializar_cola(ColaBarcos *cola);

// Agregar un barco a la cola
void agregar_a_cola(ColaBarcos *cola, Barco *barco);

// Algoritmos de calendarización
Barco *obtener_siguiente_barco_fcfs(ColaBarcos *cola);
Barco *obtener_siguiente_barco_rr(ColaBarcos *cola);
Barco *obtener_siguiente_barco_tiempo_real(ColaBarcos *cola);
Barco *obtener_siguiente_barco_prioridad(ColaBarcos *cola);
Barco *obtener_siguiente_barco_sjf(ColaBarcos *cola);

#endif // CALENDARIZACION_H