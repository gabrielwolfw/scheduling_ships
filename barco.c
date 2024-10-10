#include "barco.h"
#include <stdio.h>

#define MAX_BARCOS 100 // Definir el número máximo de barcos

// Función para agregar un barco
void agregar_barco(Barco* barcos, int id, int direccion, TipoBarco tipo, int longitud_canal) {
    if (id < 0 || id >= MAX_BARCOS) {
        printf("Error: ID de barco inválido.\n");
        return;
    }

    barcos[id].id = id;
    barcos[id].direccion = direccion;
    barcos[id].tipo = tipo;

    // Ajustar la velocidad y deadline según el tipo de barco
    switch (tipo) {
        case NORMAL:
            barcos[id].velocidad = 1.5; // Ejemplo: 1.5 unidades/segundo para barco normal
            barcos[id].deadline = 10;
            break;
        case PESQUERO:
            barcos[id].velocidad = 2.0; // Ejemplo: 2.0 unidades/segundo para barco pesquero
            barcos[id].deadline = 8;
            break;
        case PATRULLA:
            barcos[id].velocidad = 3.0; // Ejemplo: 3 unidad/segundo para barco patrullero
            barcos[id].deadline = 5;
            break;
        default:
            barcos[id].velocidad = 1.5; // Valor por defecto
            barcos[id].deadline = 10;
            break;
    }
    
    // Inicializar el tiempo restante
    barcos[id].tiempo_restante = longitud_canal / barcos[id].velocidad;
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
