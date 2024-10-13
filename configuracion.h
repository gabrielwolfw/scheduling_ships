#ifndef CONFIGURACION_CANAL_H
#define CONFIGURACION_CANAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "canal.h"  // Incluir canal.h para usar ModoControlFlujo
#include "calendarizacion.h"  // Incluir calendarizacion.h para usar AlgoritmoCalendarizacion

// Estructura para almacenar la configuración del canal
typedef struct {
    char control_flujo[20];      // Modo de control de flujo (MODO_LETRERO, MODO_EQUIDAD, etc.)
    int longitud_canal;          // Longitud del canal en unidades
    float velocidad_normal;      // Velocidad de los barcos tipo Normal
    float velocidad_pesquero;    // Velocidad de los barcos tipo Pesquero
    float velocidad_patrulla;    // Velocidad de los barcos tipo Patrulla
    int cantidad_barcos;         // Número total de barcos en la simulación
    int tiempo_letrero;          // Tiempo que tarda en cambiar el letrero
    int parametro_w;             // Parámetro W para el modo de equidad
    char algoritmo[20];          // Algoritmo de calendarización (ROUND_ROBIN, FCFS, etc.)
    int quantum;                 // Quantum para el algoritmo Round Robin
} ConfiguracionCanal;

// Función para leer la configuración del archivo
void leer_configuracion(const char *nombre_archivo, ConfiguracionCanal *config);

// Función para obtener el modo de control de flujo a partir de una cadena de texto
ModoControlFlujo obtener_modo_control_flujo(const char* control_flujo);

// Función para obtener el algoritmo de calendarización a partir de una cadena de texto
AlgoritmoCalendarizacion obtener_algoritmo_calendarizacion(const char* algoritmo);

#endif // CONFIGURACION_CANAL_H
