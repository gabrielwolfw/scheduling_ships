
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdio.h>
#include "../CEThreads.h"  

enum Algorithm
{
    RR = 1,
    PRIORITY,
    SJF,
    FCFS,
    RT
};

//Higher number is higher priority
enum Priority{
    LOW = 1,
    MEDIUM,
    HIGH
};

// Estructura para CEthread
typedef struct
{
    int id;
    int burst_time;
    enum Priority priority;
    CEthread_t thread;
} Process;

Process *schedule(Process threads[], int thread_number, enum Algorithm algorithm);

Process *SJF_schedule(Process threads[], int thread_number);

Process *RR_schedule(Process threads[], int thread_number);

Process *FCFS_schedule(Process threads[], int thread_number);

Process *PRIORITY_schedule(Process threads[], int thread_number);

Process *RT_schedule(Process threads[], int thread_number);

#endif