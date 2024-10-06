#ifndef BARCO_H
#define BARCO_H

#include "CEThreads.h" // Asegúrate de incluir la biblioteca de hilos

// Definición del tipo de barco
typedef enum {
    NORMAL,   // Barco normal
    PESQUERA, // Barco pesquero
    PATRULLA  // Barco de patrulla
} TipoBarco;

// Definición de la estructura Barco
typedef struct {
    int id;           // Identificador del barco
    int direccion;    // 0: izquierda a derecha, 1: derecha a izquierda
    int velocidad;    // Tiempo que tarda en cruzar (en segundos)
    TipoBarco tipo;   // Tipo de barco
} Barco;

// Prototipos de funciones
void agregar_barco(Barco* barcos, int id, int direccion, TipoBarco tipo);
void mostrar_info_barco(const Barco* barco); // Función para mostrar información del barco

#endif // BARCO_H
