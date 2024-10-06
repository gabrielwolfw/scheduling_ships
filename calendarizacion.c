#include "calendarizacion.h"
#include <stdio.h>

// Inicializar la cola de barcos
void inicializar_cola(ColaBarcos *cola)
{
    cola->count = 0; // Inicializa el contador de barcos
}

// Agregar un barco a la cola
void agregar_a_cola(ColaBarcos *cola, Barco *barco)
{
    if (cola->count < MAX_BARCOS)
    {
        cola->barcos[cola->count++] = barco; // Agregar el barco a la cola
    }
    else
    {
        printf("Cola de barcos llena.\n");
    }
}

// Obtener el siguiente barco según FCFS (First-Come, First-Served)
Barco *obtener_siguiente_barco_fcfs(ColaBarcos *cola)
{
    if (cola->count == 0)
    {
        return NULL; // No hay barcos en la cola
    }
    Barco *siguiente = cola->barcos[0]; // Tomar el primer barco de la cola

    // Desplazar la cola hacia adelante
    for (int i = 1; i < cola->count; i++)
    {
        cola->barcos[i - 1] = cola->barcos[i];
    }
    cola->count--; // Reducir el conteo
    return siguiente;
}

// Implementaciones adicionales para otros algoritmos (ejemplo Round Robin, Prioridad, etc.)
// Por ejemplo, para Round Robin:d
Barco *obtener_siguiente_barco_rr(ColaBarcos *cola)
{
    // Implementar la lógica de Round Robin aquí
    return NULL;
}

Barco *obtener_siguiente_barco_prioridad(ColaBarcos *cola)
{
 //Burst time es calculado con velocidad
    if (cola->count == 0)
    {
        return NULL; // No hay barcos en la cola
    }

    int i, j, index = 0;
    Barco *temp = cola->barcos[0];
     // Ordenar segun la velocidad
    for (i = 0; i < cola->count; i++) {
        index = i;
        for (j = i + 1; j < cola->count; j++)
            if (cola->barcos[j]->tipo > cola->barcos[index]->tipo)
                index = j;
        temp = cola->barcos[i];
        cola->barcos[i] = cola->barcos[index];
        cola->barcos[index] = temp;
    } 

    Barco *siguiente = cola->barcos[0]; 
    // Desplazar la cola hacia adelante
    for (int i = 1; i < cola->count; i++)
    {
        cola->barcos[i - 1] = cola->barcos[i];
    }
    cola->count--; // Reducir el conteo

    return siguiente;
}

Barco *obtener_siguiente_barco_sjf(ColaBarcos *cola)
{
    //Burst time es calculado con velocidad
    if (cola->count == 0)
    {
        return NULL; // No hay barcos en la cola
    }

    int i, j, index = 0;
    Barco *temp = cola->barcos[0];
     // Ordenar segun la velocidad
    for (i = 0; i < cola->count; i++) {
        index = i;
        for (j = i + 1; j < cola->count; j++)
            if (cola->barcos[j]->velocidad < cola->barcos[index]->velocidad)
                index = j;
        temp = cola->barcos[i];
        cola->barcos[i] = cola->barcos[index];
        cola->barcos[index] = temp;
    } 

    Barco *siguiente = cola->barcos[0]; 
    // Desplazar la cola hacia adelante
    for (int i = 1; i < cola->count; i++)
    {
        cola->barcos[i - 1] = cola->barcos[i];
    }
    cola->count--; // Reducir el conteo

    return siguiente;
}
