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
#include <linux/sched.h>
#include <sys/syscall.h>
#include <semaphore.h>

// Estructura para CEthread
typedef struct {
    int id;
    void *(*start_routine)(void *);
    void *arg;
    void *stack;
    int status;
    void *retval;
    int parent_tid;
} CEthread_t;

// Estructura para CEmutex
typedef struct {
    sem_t semaphore;
} CEmutex_t;

// Funciones públicas para CEthread
int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg);
int CEthread_join(CEthread_t *thread, void **retval);
int CEthread_end(CEthread_t *thread);


// Funciones públicas para mutex
int CEmutex_init(CEmutex_t *mutex);
int CEmutex_lock(CEmutex_t *mutex);
int CEmutex_unlock(CEmutex_t *mutex);
int CEmutex_destroy(CEmutex_t *mutex);

#endif // CETHREADS_H
