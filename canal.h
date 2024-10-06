#ifndef CANAL_H
#define CANAL_H

#include "CEThreads.h"
#include "barco.h"  // Incluir la definición de la estructura Barco

// Variables globales para controlar el canal
extern int sentido_actual;    // 0: izquierda a derecha, 1: derecha a izquierda
extern int tiempo_letrero;    // Tiempo que tarda en cambiar el letrero
extern int longitud_canal;    // Longitud del canal
extern CEmutex_t canal_mutex;
extern CEmutex_t letrero_mutex;

// Funciones para el manejo del canal
void iniciar_canal(int tiempo_letrero_definido, int longitud_definida);  // Inicializa el canal y las variables
void* cruzar_canal(void* arg);  // Función para que un barco cruce el canal
void cambiar_sentido();  // Cambia el sentido del letrero

#endif  // CANAL_H
