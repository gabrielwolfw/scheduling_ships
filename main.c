#include <stdio.h>
#include <pthread.h>
#include "CEThreads.h"  

void *print_message(void *ptr) {
    char *message = (char *)ptr;
    printf("%s\n", message);
    return NULL;
}

void test_thread_creation_and_join() {
    // --- Comparación con Pthreads ---
    pthread_t pthread1, pthread2;
    char *message1 = "Pthread - Hilo 1";
    char *message2 = "Pthread - Hilo 2";

    // Crear hilos usando Pthreads
    pthread_create(&pthread1, NULL, print_message, (void *)message1);
    pthread_create(&pthread2, NULL, print_message, (void *)message2);

    // Unir hilos Pthreads
    pthread_join(pthread1, NULL);
    pthread_join(pthread2, NULL);

    // --- Comparación con CEthreads ---
    CEthread_t cethread1, cethread2;
    char *message3 = "CEthread - Hilo 1";
    char *message4 = "CEthread - Hilo 2";

    // Crear hilos usando CEthreads
    CEthread_create(&cethread1, print_message, (void *)message3);
    CEthread_create(&cethread2, print_message, (void *)message4);

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

int main() {
    test_thread_creation_and_join();
    test_thread_mutex();
    return 0;
}