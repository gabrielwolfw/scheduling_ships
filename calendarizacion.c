#include "calendarizacion.h"
#include <stdio.h>
#include <limits.h>

// Inicializar la cola de barcos
void inicializar_cola(ColaBarcos *cola)
{
    cola->count = 0;
    cola->quantum = 5; // Valor por defecto para Round Robin
}

// Agregar un barco a la cola
void agregar_a_cola(ColaBarcos *cola, Barco *barco)
{
    if (cola->count < MAX_BARCOS)
    {
        cola->barcos[cola->count++] = barco;
    }
    else
    {
        printf("Cola de barcos llena.\n");
    }
}

// FCFS (First-Come, First-Served)
Barco *obtener_siguiente_barco_fcfs(ColaBarcos *cola)
{
    if (cola->count == 0)
        return NULL;

    Barco *siguiente = cola->barcos[0];
    for (int i = 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}

// Round Robin
Barco *obtener_siguiente_barco_rr(ColaBarcos *cola)
{
    if (cola->count == 0)
        return NULL;

    Barco *siguiente = cola->barcos[0];
    for (int i = 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    
    if (siguiente->tiempo_restante > cola->quantum)
    {
        siguiente->tiempo_restante -= cola->quantum;
        cola->barcos[cola->count - 1] = siguiente;
    }
    else
    {
        cola->count--;
    }
    return siguiente;
}

// Tiempo Real
Barco *obtener_siguiente_barco_tiempo_real(ColaBarcos *cola)
{
    if (cola->count == 0)
        return NULL;

    int index_menor_deadline = 0;
    for (int i = 1; i < cola->count; i++)
    {
        if (cola->barcos[i]->deadline < cola->barcos[index_menor_deadline]->deadline)
            index_menor_deadline = i;
    }

    Barco *siguiente = cola->barcos[index_menor_deadline];
    for (int i = index_menor_deadline + 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}

// Prioridad
Barco *obtener_siguiente_barco_prioridad(ColaBarcos *cola)
{
    if (cola->count == 0)
        return NULL;

    int index_mayor_prioridad = 0;
    for (int i = 1; i < cola->count; i++)
    {
        if (cola->barcos[i]->tipo > cola->barcos[index_mayor_prioridad]->tipo)
            index_mayor_prioridad = i;
    }

    Barco *siguiente = cola->barcos[index_mayor_prioridad];
    for (int i = index_mayor_prioridad + 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}

// SJF (Shortest Job First)
Barco *obtener_siguiente_barco_sjf(ColaBarcos *cola)
{
    if (cola->count == 0)
        return NULL;

    int index_menor_tiempo = 0;
    for (int i = 1; i < cola->count; i++)
    {
        if (cola->barcos[i]->velocidad > cola->barcos[index_menor_tiempo]->velocidad)
            index_menor_tiempo = i;
    }

    Barco *siguiente = cola->barcos[index_menor_tiempo];
    for (int i = index_menor_tiempo + 1; i < cola->count; i++)
        cola->barcos[i - 1] = cola->barcos[i];
    cola->count--;
    return siguiente;
}