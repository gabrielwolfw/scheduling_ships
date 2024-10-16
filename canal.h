#ifndef CANAL_H
#define CANAL_H

#include "CEThreads.h"
#include "barco.h"
#include "calendarizacion.h"
#include <stdbool.h>

// Enum para los modos de control de flujo
typedef enum {
    MODO_LETRERO,
    MODO_EQUIDAD,
    MODO_TICO
} ModoControlFlujo;

// Variables globales para controlar el canal
extern int sentido_actual;    // 0: izquierda a derecha, 1: derecha a izquierda
extern int tiempo_letrero;    // Tiempo que tarda en cambiar el letrero
extern int longitud_canal;    // Longitud del canal
extern int parametro_w;       // Número de barcos que deben pasar de cada lado en modo equidad
extern int barcos_pasados;    // Contador de barcos que han pasado en el sentido actual
extern ModoControlFlujo modo_actual;  // Modo actual de control de flujo
extern AlgoritmoCalendarizacion algoritmo_actual; // Algoritmo de calendarización actual
extern CEmutex_t canal_mutex;
extern CEmutex_t letrero_mutex;
extern SistemaCalendarizacion sistema_cal; // Sistema de calendarización


extern bool canal_activo;  // Agregar esto para controlar el estado del canal

// Funciones para el manejo del canal
void iniciar_canal(int tiempo_letrero_definido, int longitud_definida, ModoControlFlujo modo, int parametro_w_definido, AlgoritmoCalendarizacion algoritmo);
void* cruzar_canal(void* arg);  // Función general para que un barco cruce el canal
void* cambiar_sentido();  // Cambia el sentido del letrero
void agregar_barco_al_canal(Barco* barco);

// Funciones específicas para cada modo de control de flujo
void* cruzar_canal_letrero(void* arg);
void* cruzar_canal_equidad(void* arg);
void* cruzar_canal_tico(void* arg);
int *obtener_cruzados();
void registrar_cruce(int id);

#endif // CANAL_H