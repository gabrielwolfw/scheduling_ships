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

int sort_threads(Process threads[], enum Algorithm algorithm)
{
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
        break;
    case 4:
        printf("%s\n", "Algorithm selected: FCFS");
        break;
    case 5:
        printf("%s\n", "Algorithm selected: RT");
        break;
    }
    return 0;
}

Process *SJF_cal(Process threads[], int thread_number)
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

int main(){
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
    Process *ordered_processes = SJF_cal(process_list, thread_number);

    for(int i = 0; i < 3; i++){
        printf("Process id: %d and BT: %d \n", ordered_processes[i].id, ordered_processes[i].burst_time);
    }
    return 0;
}
