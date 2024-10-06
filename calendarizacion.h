#ifndef CALENDARIZACION_H
#define CALENDARIZACION_H

#include "barco.h"  // Asegúrate de incluir la definición de los barcos

#define MAX_BARCOS 6 // Número máximo de barcos

// Definición de la estructura ColaBarcos
typedef struct {
    Barco* barcos[MAX_BARCOS];  // Array para almacenar punteros a barcos
    int count;                  // Contador de barcos en la cola
} ColaBarcos;

// Prototipos de funciones para manejar la cola de barcos y la planificación
void inicializar_cola(ColaBarcos* cola);
void agregar_a_cola(ColaBarcos* cola, Barco* barco);
Barco* obtener_siguiente_barco_fcfs(ColaBarcos* cola);  // FCFS (First-Come, First-Served)
// Prototipos adicionales para otros algoritmos de calendarización
Barco* obtener_siguiente_barco_rr(ColaBarcos* cola);   // Round Robin
Barco* obtener_siguiente_barco_prioridad(ColaBarcos* cola);  // Prioridad
Barco* obtener_siguiente_barco_sjf(ColaBarcos* cola);   // Shortest Job First

#endif // CALENDARIZACION_H
