#include "canal.h"
#include "CEThreads.h"
#include <stdio.h>

#define MAX_BARCOS 5  // Número máximo de barcos

int main() {
    CEthread_t hilos[MAX_BARCOS];
    CEthread_t hilo_letrero;  // Hilo para cambiar el letrero
    Barco barcos[MAX_BARCOS];

    // Iniciar el canal con un cambio de letrero cada 5 segundos
    iniciar_canal(5);
    printf("Canal iniciado con cambio de letrero cada 5 segundos.\n");

    // Crear barcos en direcciones alternas
    agregar_barco(barcos, 0, 0, 2);  // Barco 0, izquierda a derecha, tarda 2 segundos en cruzar
    agregar_barco(barcos, 1, 1, 3);  // Barco 1, derecha a izquierda, tarda 3 segundos en cruzar
    agregar_barco(barcos, 2, 0, 1);  // Barco 2, izquierda a derecha, tarda 1 segundo en cruzar
    agregar_barco(barcos, 3, 1, 4);  // Barco 3, derecha a izquierda, tarda 4 segundos en cruzar
    agregar_barco(barcos, 4, 0, 2);  // Barco 4, izquierda a derecha, tarda 2 segundos en cruzar
    printf("Barcos agregados a la simulación.\n");

    // Iniciar el hilo del letrero para cambiar el sentido del canal
    if (CEthread_create(&hilo_letrero, (void*) cambiar_sentido, NULL) != 0) {
        printf("Error al crear el hilo del letrero.\n");
        return -1;
    }
    printf("Hilo del letrero iniciado.\n");

    // Crear los hilos para los barcos
    for (int i = 0; i < MAX_BARCOS; i++) {
        if (CEthread_create(&hilos[i], cruzar_canal, (void*) &barcos[i]) != 0) {
            printf("Error al crear el hilo para el barco %d.\n", i);
            return -1;
        }
    }
    printf("Hilos para los barcos iniciados.\n");

    // Esperar a que todos los barcos crucen el canal
    for (int i = 0; i < MAX_BARCOS; i++) {
        if (CEthread_join(&hilos[i], NULL) != 0) {
            printf("Error al unir el hilo del barco %d.\n", i);
            return -1;
        }
        printf("Barco %d ha cruzado el canal correctamente.\n", i);
    }

    // Terminar el hilo del letrero (esto puede ser opcional dependiendo de cómo lo manejes)
    if (CEthread_end(&hilo_letrero) != 0) {
        printf("Error al terminar el hilo del letrero.\n");
        return -1;
    }
    printf("Hilo del letrero terminado.\n");

    // Destruir los recursos (mutex)
    CEmutex_destroy(&canal_mutex);
    CEmutex_destroy(&letrero_mutex);
    printf("Recursos de sincronización destruidos.\n");

    printf("Simulación terminada correctamente.\n");
    return 0;
}
