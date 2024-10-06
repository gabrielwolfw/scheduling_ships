#include "Scheduler.h"

//Thread number is the amount of threads in the list
Process *schedule(Process threads[], int thread_number, enum Algorithm algorithm)
{
    Process *ordered_processes;
    switch (algorithm)
    {
    case 1:
        printf("%s\n", "Algorithm selected: RR");
        break;
    case 2:
        printf("%s\n", "Algorithm selected: PRIORITY");
        break;
    case 3:
        printf("%s\n", "Algorithm selected: SJF");
        ordered_processes = SJF_schedule(threads, thread_number);
        return ordered_processes;
    case 4:
        printf("%s\n", "Algorithm selected: FCFS");
        break;
    case 5:
        printf("%s\n", "Algorithm selected: RT");
        break;
    default:
        break;
    }
    return 0;
}

Process *SJF_schedule(Process threads[], int thread_number)
{
    int i, j, index = 0;
    Process temp = threads[0];
     // Sorting process according to their Burst Time.
    for (i = 0; i < thread_number; i++) {
        index = i;
        for (j = i + 1; j < thread_number; j++)
            if (threads[j].burst_time < threads[index].burst_time)
                index = j;
        temp = threads[i];
        threads[i] = threads[index];
        threads[index] = temp;
    } 
    return threads;
}
