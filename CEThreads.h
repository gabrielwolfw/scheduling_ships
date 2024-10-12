#ifndef CETHREADS_H
#define CETHREADS_H

#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
//#include <linux/sched.h>
#include <sched.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

// Estructura para CEthread
typedef struct {
    int id;
    void *(*start_routine)(void *);
    void *arg;
    void *stack;
    int status;
    void *retval;
    int parent_tid;
    int barco_id;  // Nuevo campo para almacenar el ID del barco
} CEthread_t;

// Estructura para CEmutex
typedef struct {
    sem_t semaphore;
} CEmutex_t;

// Funciones públicas para CEthread
int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg, int barco_id);
int CEthread_join(CEthread_t *thread, int *barco_id);
int CEthread_end(CEthread_t *thread);
void CEthread_sleep(int segundos);
void CEthread_yield(void);
void CEthread_init(void);

// Funciones públicas para mutex
int CEmutex_init(CEmutex_t *mutex);
int CEmutex_lock(CEmutex_t *mutex);
int CEmutex_unlock(CEmutex_t *mutex);
int CEmutex_destroy(CEmutex_t *mutex);

#endif // CETHREADS_H