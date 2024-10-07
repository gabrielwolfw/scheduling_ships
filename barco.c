#include "barco.h"
#include "canal.h"
#include <stdio.h>

// Función para agregar un barco
void agregar_barco(Barco* barcos, int id, int direccion, TipoBarco tipo) {
    barcos[id].id = id;
    barcos[id].direccion = direccion;

    // Ajustar la velocidad según el tipo de barco
    switch (tipo) {
        case NORMAL:
            barcos[id].velocidad = 2.0; // Ejemplo: 2 unidades/segundo para barco normal
            barcos[id].tipo = NORMAL;  // Asignar el tipo de barco
            break;
        case PESQUERO:
            barcos[id].velocidad = 1.5; // Ejemplo: 1.5 unidades/segundo para barco pesquero
            barcos[id].tipo = PESQUERO; // Asignar el tipo de barco
            break;
        case PATRULLA:
            barcos[id].velocidad = 1.0; // Ejemplo: 1 unidad/segundo para barco patrullero
            barcos[id].tipo = PATRULLA; // Asignar el tipo de barco
            break;
        default:
            barcos[id].velocidad = 2.0; // Valor por defecto
            barcos[id].tipo = NORMAL;  // Asignar tipo por defecto
            break;
    }
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

    printf("Barco ID: %d, Tipo: %s, Dirección: %s, Velocidad: %.2f unidades/segundo\n",
           barco->id, tipo_str, direccion_str, barco->velocidad);
}
