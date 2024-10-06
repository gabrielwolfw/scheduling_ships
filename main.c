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

void schedule_threads(void){
    // --- Comparación con CEthreads ---
    CEthread_t cethread1, cethread2;
    char *message3 = "CEthread - Hilo 1";
    char *message4 = "CEthread - Hilo 2";

    // Crear hilos usando CEthreads
    CEthread_create(&cethread1, print_message, (void *)message3);
    CEthread_create(&cethread2, print_message, (void *)message4);
}

int main() {
    test_thread_creation_and_join();
    return 0;
}