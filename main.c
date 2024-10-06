#include <stdio.h>
#include <pthread.h>
#include "CEThreads.h"  
#include "Utils/Scheduler.h"

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
    Process s1, s2, s3;
    s1.id = 1;
    s1.burst_time = 10;
    s1.status = 0;

    s2.id = 2;
    s2.burst_time = 13;
    s2.status = 0;

    s3.id = 3;
    s3.burst_time = 3;
    s3.status = 0;

    Process process_list[3] = {s1, s2, s3};  
    int thread_number = 3;
    Process *ordered_processes = schedule(process_list, thread_number, SJF);

    for(int i = 0; i < 3; i++){
        printf("Process id: %d and BT: %d \n", ordered_processes[i].id, ordered_processes[i].burst_time);
    }
    return 0;
}