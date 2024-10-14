#include "canal.h"
#include "barco.h"
#include "CEThreads.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_BARCOS 6 // Número de barcos para la prueba

void *chequear_colas()
{
    while (1)
    {
        int *lista_cruzados = obtener_cruzados();
 
        if (lista_cruzados != NULL)
        {
            for (int i = 0; i < 6; i++)
            {
                if (lista_cruzados[i] != 9)
                {
                    printf("Ya cruzo: %d \n", lista_cruzados[i]);
                }
            }
            usleep(1000000);
        }
    }
}

void crear_barco(Barco *barcos, int id, int direccion, TipoBarco tipo, int longitud_canal)
{
    inicializar_barco(&barcos[id], id, direccion, tipo, longitud_canal);
    mostrar_info_barco(&barcos[id]);
    agregar_barco_al_canal(&barcos[id]);
}

int main()
{
    // Configuración inicial del canal
    int tiempo_letrero = 10;                          // Tiempo en segundos para cambiar el letrero
    int longitud_canal = 10;                          // Longitud del canal en unidades
    int parametro_w = 3;                              // Número de barcos por dirección en modo equidad
    AlgoritmoCalendarizacion algoritmo = ROUND_ROBIN; // Cambiar a ROUND_ROBIN, SJF, etc. si es necesario
    ModoControlFlujo modo = MODO_LETRERO;
    int quantum = 5; // Quantum para Round Robin


    // Inicialización del canal
    iniciar_canal(tiempo_letrero, longitud_canal, modo, parametro_w, algoritmo);
    sistema_cal.quantum = quantum; // Establecer el quantum para Round Robin

    // Mostrar configuración del canal
    printf("Configuración del canal:\n");
    printf("- Tiempo de letrero: %d segundos\n", tiempo_letrero);
    printf("- Longitud del canal: %d unidades\n", longitud_canal);
    printf("- Algoritmo de calendarización: %s\n",
           algoritmo == ROUND_ROBIN ? "ROUND_ROBIN" : algoritmo == FCFS      ? "FCFS"
                                                  : algoritmo == SJF         ? "SJF"
                                                  : algoritmo == PRIORIDAD   ? "PRIORIDAD"
                                                  : algoritmo == TIEMPO_REAL ? "TIEMPO_REAL"
                                                                             : "DESCONOCIDO");
    printf("- Quantum (para Round Robin): %d segundos\n", quantum);

    Barco barcos[NUM_BARCOS];
    printf("\nAgregando barcos a la cola...\n");

    // Barcos de prueba con diferentes tipos
    crear_barco(barcos, 0, 1, NORMAL, longitud_canal);   // Barco 0, dirección derecha
    crear_barco(barcos, 1, 1, PESQUERO, longitud_canal); // Barco 1, dirección derecha
    crear_barco(barcos, 2, 0, PATRULLA, longitud_canal); // Barco 2, dirección izquierda
    crear_barco(barcos, 3, 0, NORMAL, longitud_canal);   // Barco 3, dirección izquierda
    crear_barco(barcos, 4, 0, NORMAL, longitud_canal);   // Barco 4, dirección izquierda
    crear_barco(barcos, 5, 1, PATRULLA, longitud_canal); // Barco 5, dirección derecha

    // Crear el hilo para el cambio de sentido del letrero
    CEthread_t hilo_cambio_sentido;
    if (modo == MODO_LETRERO)
    {
        canal_activo = true;
        if (CEthread_create(&hilo_cambio_sentido, (void *)cambiar_sentido, NULL, -1) != 0)
        {
            fprintf(stderr, "Error al crear hilo para cambio de sentido\n");
            return 1;
        }
    }

    CEthread_t monitorear_Cruce;
    CEthread_create(&monitorear_Cruce, (void *)chequear_colas, NULL, 0);

    // Procesar el cruce de los barcos en el canal
    CEthread_t hilos_barcos[NUM_BARCOS];
    for (int i = 0; i < NUM_BARCOS; i++)
    {
        if (CEthread_create(&hilos_barcos[i], cruzar_canal, &barcos[i], barcos[i].id) != 0)
        {
            fprintf(stderr, "Error al crear hilo para el barco %d\n", i);
            return 1;
        }
    }

    // Esperar a que todos los barcos crucen
    for (int i = 0; i < NUM_BARCOS; i++)
    {
        int barco_id_cruzado;
        CEthread_join(&hilos_barcos[i], &barco_id_cruzado);
        printf("El barco con ID %d ha cruzado el canal\n", barco_id_cruzado);
    }

    // Detener el hilo de cambio de sentido si está activo
    if (modo == MODO_LETRERO)
    {
        canal_activo = false;
        CEthread_join(&hilo_cambio_sentido, NULL);
    }

    printf("\nSimulación completada. Todos los barcos han cruzado el canal.\n");

    return 0;
}