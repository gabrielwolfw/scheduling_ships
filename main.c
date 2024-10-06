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

void schedule_threads(Process processes[], int process_number, enum Algorithm algorithm){
    Process *ordered_processes = schedule(processes, process_number, algorithm);
    printf("%s\n", "Thread schedule: \n");
    for(int i = 0; i < process_number; i++){
        printf("Process id: %d and BT: %d \n", ordered_processes[i].id, ordered_processes[i].burst_time);
    }
    for(int i = 0; i < process_number; i++){
        CEthread_create(&ordered_processes[i].thread, print_message, "Welcome from running process: " + ordered_processes[i].id);
        //CEthread_join(&ordered_processes[i].thread, NULL);
    }
}

int main() {
    CEthread_t cethread1, cethread2, cethread3;

    Process s1, s2, s3;
    s1.id = 1;
    s1.burst_time = 10;
    s1.thread = cethread1;

    s2.id = 2;
    s2.burst_time = 13;
    s2.thread = cethread2;

    s3.id = 3;
    s3.burst_time = 3;
    s3.thread = cethread3;

    Process process_list[3] = {s1, s2, s3};  
    int thread_number = 3;

    schedule_threads(process_list, thread_number, SJF);

    return 0;
}