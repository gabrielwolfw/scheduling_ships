#ifndef CANAL_H
#define CANAL_H

#include "CEThreads.h"

// Definición de la estructura Barco
typedef struct {
    int id;           // Identificador del barco
    int direccion;    // 0: izquierda a derecha, 1: derecha a izquierda
    int tiempo_paso;  // Tiempo que tarda en cruzar el canal
} Barco;

// Variables globales para controlar el canal
extern int sentido_actual;    // 0: izquierda a derecha, 1: derecha a izquierda
extern int tiempo_letrero;    // Tiempo que tarda en cambiar el letrero
extern CEmutex_t canal_mutex;
extern CEmutex_t letrero_mutex;

// Funciones para el manejo del canal
void iniciar_canal(int tiempo_letrero_definido);  // Inicializa el canal y las variables
void agregar_barco(Barco* barcos, int id, int direccion, int tiempo_paso);  // Agrega un barco
void* cruzar_canal(void* arg);  // Función para que un barco cruce el canal
void cambiar_sentido();  // Cambia el sentido del letrero

#endif
