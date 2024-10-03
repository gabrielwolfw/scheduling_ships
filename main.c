#include <stdio.h>
#include <pthread.h>
#include "CEThreads.h"  

CEmutex_t print_mutex; // Mutex global para la impresión
void *print_message(void *ptr) {
    char *message = (char *)ptr;
    CEmutex_lock(&print_mutex);  // Bloquear el mutex antes de imprimir
    printf("%s\n", message);
    CEmutex_unlock(&print_mutex); // Desbloquear el mutex después de imprimir
    return NULL;
}

void test_thread_creation_and_join() {
    // --- Comparación con Pthreads ---
    pthread_t pthread1, pthread2;
    char *message1 = "Pthread - Hilo 1";
    char *message2 = "Pthread - Hilo 2";

    // Crear hilos usando Pthreads
    if (pthread_create(&pthread1, NULL, print_message, (void *)message1) != 0) {
        perror("Error creando el Pthread 1");
    }
    if (pthread_create(&pthread2, NULL, print_message, (void *)message2) != 0) {
        perror("Error creando el Pthread 2");
    }

    // Unir hilos Pthreads
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);

    // --- Comparación con CEthreads ---
    CEthread_t cethread1, cethread2;
    char *message3 = "CEthread - Hilo 1";
    char *message4 = "CEthread - Hilo 2";

    // Crear hilos usando CEthreads
    if (CEthread_create(&cethread1, print_message, (void *)message3) != 0) {
        perror("Error creando el CEthread 1");
    }
    if (CEthread_create(&cethread2, print_message, (void *)message4) != 0) {
        perror("Error creando el CEthread 2");
    }

    // Unir hilos CEthreads
    CEthread_join(&cethread1, NULL);
    CEthread_join(&cethread2, NULL);
}


void test_thread_mutex() {
    // Test CEmutex
    CEmutex_t ce_mutex;
    int ce_result;

    printf("Testing CEmutex:\n");

    ce_result = CEmutex_init(&ce_mutex);
    printf("CEmutex_init result: %d\n", ce_result);

    ce_result = CEmutex_lock(&ce_mutex);
    printf("CEmutex_lock result: %d\n", ce_result);

    ce_result = CEmutex_unlock(&ce_mutex);
    printf("CEmutex_unlock result: %d\n", ce_result);

    ce_result = CEmutex_destroy(&ce_mutex);
    printf("CEmutex_destroy result: %d\n", ce_result); // Corrected to include the return value

    // Test pthread_mutex
    pthread_mutex_t pthread_mutex;
    int pthread_result;

    printf("Testing pthread_mutex:\n");

    pthread_result = pthread_mutex_init(&pthread_mutex, NULL);
    printf("pthread_mutex_init result: %d\n", pthread_result);

    pthread_result = pthread_mutex_lock(&pthread_mutex);
    printf("pthread_mutex_lock result: %d\n", pthread_result);

    pthread_result = pthread_mutex_unlock(&pthread_mutex);
    printf("pthread_mutex_unlock result: %d\n", pthread_result);

    pthread_result = pthread_mutex_destroy(&pthread_mutex);
    printf("pthread_mutex_destroy result: %d\n", pthread_result); // Corrected to include the return value

    printf("\nMutex tests completed.\n");
}

void *long_running_task_pthread(void *arg) {
    int *count = (int *)arg;
    while (*count < 5) {
        printf("Pthread en ejecución: %d\n", *count);
        (*count)++;
        sleep(1);  // Simular trabajo
    }
    return NULL;
}

int test_pthread_end() {
    pthread_t pthread;
    int count = 0;

    // Crear el hilo
    if (pthread_create(&pthread, NULL, long_running_task_pthread, (void *)&count) != 0) {
        fprintf(stderr, "Error creando el Pthread.\n");
        return -1;
    }

    // Dejar el hilo correr hasta que complete su tarea
    sleep(5); // Allow enough time for the thread to complete

    // Terminar el hilo (usando pthread_cancel para terminarlo)
    if (pthread_cancel(pthread) != 0) {
        fprintf(stderr, "Error cancelando el Pthread.\n");
        return -1;
    }

    // Esperar a que el hilo termine
    if (pthread_join(pthread, NULL) != 0) {
        fprintf(stderr, "Error esperando al Pthread.\n");
        return -1;
    }

    printf("El Pthread ha terminado correctamente.\n");
    return 0;
}

void *long_running_task(void *arg) {
    int *count = (int *)arg;
    while (*count < 5) {
        printf("Hilo en ejecución (CEthread): %d\n", *count);
        (*count)++;
        sleep(1);  // Simular trabajo
    }
    return NULL;
}

int test_thread_end() {
    CEthread_t cethread;
    int count = 0;

    // Crear el CEthread
    if (CEthread_create(&cethread, long_running_task, (void *)&count) != 0) {
        fprintf(stderr, "Error creando el CEthread.\n");
        return -1;
    }

    // Dejar el CEthread correr por un momento
    sleep(2);

    // Terminar el CEthread
    if (CEthread_end(&cethread) != 0) {
        fprintf(stderr, "Error terminando el CEthread.\n");
        return -1;
    }

    // Verificar que el CEthread ha terminado
    if (cethread.status == 0) {
        printf("El CEthread ha terminado correctamente.\n");
    } else {
        printf("El CEthread aún está activo.\n");
    }

    return 0;
}

int main() {
    CEmutex_init(&print_mutex); // Inicializar el mutex
    test_thread_creation_and_join();
    test_thread_mutex();
    printf("\nPrueba de Pthreads:\n");
    test_pthread_end();

    printf("\nPrueba de CEthreads:\n");
    test_thread_end(); // Llamada a la prueba de CEthreads

    CEmutex_destroy(&print_mutex); // Destruir el mutex
    return 0;
}