#include <sched.h> //Clone() method
#include <stdlib.h> // malloc
#include <stdio.h>
#include <asm/signal.h>

// Estructura del hilo CEthread
typedef struct {
    int id;                            // Identificador del hilo (similar a pthread_t)
    void *(*start_routine)(void *);    // Función que ejecuta el hilo
    void *arg;                         // Argumentos pasados a la función
    void *stack;                       // Puntero a la pila asignada para el hilo
    int status;                        // Estado del hilo (corriendo, finalizado, etc.)
} CEthread_t;

// Crear hilo
int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg) {
    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->stack = malloc(1024 * 64);  // Asignar pila

    if (thread->stack == NULL) {
        return -1;  // Error asignando pila
    }

    // Crear hilo con clone() y guardar el ID en thread->id
    thread->id = clone((int (*)(void *))start_routine, thread->stack + 1024 * 64, SIGCHLD, arg);

    if (thread->id == -1) {
        free(thread->stack);
        return -1;
    }

    thread->status = 1;  // Hilo corriendo
    return 0;
}

// Unir hilo
int CEthread_join(CEthread_t *thread) {
    int status;
    waitpid(thread->id, &status, 0);  // Esperar a que el hilo termine
    free(thread->stack);              // Liberar la pila
    return 0;
}