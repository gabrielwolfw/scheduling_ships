// barco.h
#ifndef BARCO_H
#define BARCO_H

#include "CEThreads.h"
#include <stdio.h>
#include <stdlib.h>

// Definición de tipos de barco
typedef enum {
    NORMAL,
    PESQUERA,
    PATRULLA
} TipoBarco;

// Estructura para representar un barco
typedef struct {
    int id;                // Identificador del barco
    int position;         // Posición actual en el canal
    int speed;            // Velocidad del barco
    TipoBarco tipo;       // Tipo de barco
    CEthread_t thread;    // Hilo asociado al barco
} Barco;

// Funciones para trabajar con barcos
void inicializar_barco(Barco *barco, int id, TipoBarco tipo);
void *mover_barco(void *arg);
void liberar_barco(Barco *barco);

#endif // BARCO_H
