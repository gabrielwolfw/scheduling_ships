#include <stdio.h>
#include <pthread.h>
#include "CEThreads.h"  
#include "Utils/Scheduler.h"

void *print_message(void *ptr) {
    char *message = (char *)ptr;
    printf("%s\n", message);
    return NULL;
}

void schedule_threads(Process processes[], int process_number, enum Algorithm algorithm){
    //Schedule returns a list of processes in the order they should be executed
    Process *ordered_processes = schedule(processes, process_number, algorithm);

    char buff[32];
    //Print the execution order of the list
    printf("%s\n", "Thread schedule: ");
    for(int i = 0; i < process_number; i++){
        printf("Process id: %d \n", ordered_processes[i].id);
    }
    for(int i = 0; i < process_number; i++){
        snprintf(buff, 32, "Welcome from running process: %d", ordered_processes[i].id);
        //Executes each thread according to the ordered list
        CEthread_create(&ordered_processes[i].thread, print_message, buff);
        CEthread_join(&ordered_processes[i].thread, NULL);
    }
}

int main() {
    CEthread_t cethread1, cethread2, cethread3;

    Process s1, s2, s3;
    s1.id = 1;
    s1.burst_time = 10;
    s1.thread = cethread1;
    s1.priority = LOW;

    s2.id = 2;
    s2.burst_time = 13;
    s2.thread = cethread2;
    s2.priority = MEDIUM;

    s3.id = 3;
    s3.burst_time = 3;
    s3.thread = cethread3;
    s3.priority = HIGH;

    Process process_list[3] = {s1, s2, s3};  
    int thread_number = 3;

    //Orders the process list according to the selected algorithm and starts execution of each thread
    //Expected: 3 1 2
    schedule_threads(process_list, thread_number, SJF);
    printf("%s\n", "---------------------------------------");
    //Expected: 3 2 1
    schedule_threads(process_list, thread_number, PRIORITY);

    //printf("%s \n", "Welcome from running process: " + 1);
    return 0;
}