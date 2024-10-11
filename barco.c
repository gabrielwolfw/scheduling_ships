#include "barco.h"
#include <stdio.h>

// Función para inicializar un barco con los valores dados
void inicializar_barco(Barco* barco, int id, int direccion, TipoBarco tipo, int longitud_canal) {
    barco->id = id;
    barco->direccion = direccion;
    barco->tipo = tipo;

    // Ajustar la velocidad y deadline según el tipo de barco
    switch (tipo) {
        case NORMAL:
            barco->velocidad = 1.5; // Ejemplo: 1.5 unidades/segundo para barco normal
            barco->deadline = (int)(longitud_canal / barco->velocidad) + 2;
            barco->prioridad = 1;
            break;
        case PESQUERO:
            barco->velocidad = 2.0; // Ejemplo: 2.0 unidades/segundo para barco pesquero
            barco->deadline = (int)(longitud_canal / barco->velocidad) + 2;
            barco->prioridad = 2;
            break;
        case PATRULLA:
            barco->velocidad = 3.0; // Ejemplo: 3 unidades/segundo para barco patrullero
            barco->deadline = (int)(longitud_canal / barco->velocidad) + 2;
            barco->prioridad = 3;
            break;
        default:
            barco->velocidad = 1.5; // Valor por defecto
            barco->deadline = (int)(longitud_canal / barco->velocidad) + 2;
            barco->prioridad = 1;
            break;
    }

    // Inicializar el tiempo restante
    barco->tiempo_restante = (int)(longitud_canal / barco->velocidad);
}

// Función para mostrar información del barco
void mostrar_info_barco(const Barco* barco) {
    const char* direccion_str = barco->direccion == 0 ? "izquierda a derecha" : "derecha a izquierda";
    const char* tipo_str;

    switch (barco->tipo) {
        case NORMAL:
            tipo_str = "Normal";
            break;
        case PESQUERO:
            tipo_str = "Pesquero";
            break;
        case PATRULLA:
            tipo_str = "Patrulla";
            break;
        default:
            tipo_str = "Desconocido";
            break;
    }

    printf("Barco ID: %d, Tipo: %s, Dirección: %s, Velocidad: %.2f unidades/segundo, Tiempo restante: %d, Deadline: %d, Prioridad: %d\n",
       barco->id, tipo_str, direccion_str, barco->velocidad, barco->tiempo_restante, barco->deadline, barco->prioridad);
}
