
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdio.h>

enum Algorithm
{
    RR = 1,
    PRIORITY,
    SJF,
    FCFS,
    RT
};

// Estructura para CEthread
typedef struct
{
    int id;
    int burst_time;
    int status;
} Process;

Process *schedule(Process threads[], int thread_number, enum Algorithm algorithm);

Process *SJF_schedule(Process threads[], int thread_number);

#endif