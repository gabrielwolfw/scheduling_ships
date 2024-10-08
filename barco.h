#ifndef BARCO_H
#define BARCO_H

#include "CEThreads.h"

// Prototipo de la función main_barco
void main_barco();

typedef enum {
    NORMAL,
    PESQUERO,
    PATRULLA
} TipoBarco;

typedef struct {
    int id;
    TipoBarco tipo; // Para el algoritmo de prioridad
    float velocidad; // Para el algoritmo de SJF
    int direccion;  // 0: izquierda a derecha, 1: derecha a izquierda
    int tiempo_restante;  // Para Round Robin
    int deadline;  // Para algoritmo de Tiempo Real
} Barco;

// Prototipos de funciones
void agregar_barco(Barco* barcos, int id, int direccion, TipoBarco tipo);
void mostrar_info_barco(const Barco* barco); // Función para mostrar información del barco

#endif // BARCO_H
