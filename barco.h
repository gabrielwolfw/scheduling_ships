#ifndef BARCO_H
#define BARCO_H

#include "CEThreads.h"

// Definición del tipo de barco
typedef enum {
    NORMAL,
    PESQUERO,
    PATRULLA
} TipoBarco;

// Estructura de Barco
typedef struct Barco {
    int id;
    TipoBarco tipo;       // Elección del tipo de barco
    float velocidad;      // Para el algoritmo de SJF
    int direccion;        // 0: izquierda a derecha, 1: derecha a izquierda
    int tiempo_restante;  // Para Round Robin
    int deadline;         // Para el algoritmo de Tiempo Real
    int prioridad;        // Para el algoritmo de prioridad
    struct Barco *siguiente; // Puntero para listas enlazadas (si es necesario en otras partes del sistema)
} Barco;

// Prototipos de funciones
void inicializar_barco(Barco* barco, int id, int direccion, TipoBarco tipo, int longitud_canal); // Función para inicializar un barco
void mostrar_info_barco(const Barco* barco); // Función para mostrar información del barco

#endif // BARCO_H
