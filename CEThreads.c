#include <sched.h> 
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <linux/sched.h>
#include <semaphore.h>

// Estructura para CEthread
typedef struct {
    int id;
    void *(*start_routine)(void *);
    void *arg;
    void *stack;
    int status;
    void *retval;
    int parent_pid;
} CEthread_t;

int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg) {
    thread->start_routine = start_routine;
    thread->arg = arg;

    // Usar posix_memalign para una alineación de pila adecuada
    if (posix_memalign(&thread->stack, 16, 1024 * 64) != 0) {
        return -1;  // Error asignando pila
    }

    // Crear el hilo con clone()
    thread->id = clone((int (*)(void *))start_routine, thread->stack + 1024 * 64, 
                        CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_IO, arg);

    if (thread->id == -1) {
        perror("clone failed");
        free(thread->stack);
        return -1;
    }

    thread->status = 1;  // Hilo corriendo
    thread->parent_pid = getpid();
    return 0;
}

int CEthread_join(CEthread_t *thread) {
    int status;
    if (waitpid(thread->id, &status, 0) == -1) {
        perror("waitpid failed");
        return -1;
    }
    free(thread->stack);
    thread->status = 0;
    return 0;
}

int CEthread_end(CEthread_t *thread) {
    if (thread->status != 1) {
        return -1;
    }

    // Enviar señal para terminar el hilo
    if (kill(thread->id, SIGTERM) != 0) {
        perror("kill failed");
        return -1;
    }

    // Esperar a que el hilo termine
    int status;
    if (waitpid(thread->id, &status, 0) == -1) {
        perror("waitpid failed");
        return -1;
    }

    free(thread->stack);
    thread->status = 0;
    return 0;
}



// Estructura para CEmutex
typedef struct {
    sem_t semaphore;
} CEmutex_t;

// Inicializar el mutex
int CEmutex_init(CEmutex_t *mutex) {
    return sem_init(&mutex->semaphore, 0, 1);  // Inicializar semáforo binario
}

// Bloquear el mutex
int CEmutex_lock(CEmutex_t *mutex) {
    return sem_wait(&mutex->semaphore);  // Bloquea hasta que el semáforo esté disponible
}

// Desbloquear el mutex
int CEmutex_unlock(CEmutex_t *mutex) {
    return sem_post(&mutex->semaphore);  // Libera el semáforo
}

// Destruir el mutex
int CEmutex_destroy(CEmutex_t *mutex) {
    return sem_destroy(&mutex->semaphore);  // Destruir el semáforo
}
