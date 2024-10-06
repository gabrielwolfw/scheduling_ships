// barco.c
#include "barco.h"

void inicializar_barco(Barco *barco, int id, TipoBarco tipo) {
    barco->id = id;
    barco->position = 0;  // Inicialmente en la posición 0

    // Asignar velocidad según el tipo de barco
    switch (tipo) {
        case NORMAL:
            barco->speed = 1;  // Lenta
            break;
        case PESQUERA:
            barco->speed = 2;  // Moderada
            break;
        case PATRULLA:
            barco->speed = 3;  // Rápida
            break;
    }

    barco->tipo = tipo;
}

void *mover_barco(void *arg) {
    Barco *barco = (Barco *)arg;
    while (barco->position < 100) {  // Por ejemplo, avanzar hasta la posición 100
        barco->position += barco->speed;  // Mover el barco según su velocidad
        printf("Barco %d (Tipo: %d) en la posición %d\n", barco->id, barco->tipo, barco->position);
        CEthread_sleep(1);  // Dormir para simular el tiempo de movimiento
    }
    return NULL;
}

void liberar_barco(Barco *barco) {
    // Aquí puedes agregar lógica para liberar recursos si es necesario
}
