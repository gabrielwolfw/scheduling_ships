#include "CEThreads.h"
#define STACK_SIZE (1024 * 64)


volatile sig_atomic_t keep_running = 1; // Variable global para controlar la ejecución

void handle_signal(int signum) {
    if (signum == SIGTERM) {
        keep_running = 0; // Indicar que el hilo debe terminar
    }
}
static int thread_start_wrapper(void *arg) {
    CEthread_t *thread = (CEthread_t *)arg;
    thread->retval = thread->start_routine(thread->arg);
    return 0;
}

int CEthread_create(CEthread_t *thread, void *(*start_routine)(void *), void *arg) {
    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->stack = malloc(STACK_SIZE);
    
    if (!thread->stack) {
        perror("Failed to allocate stack");
        return -1;
    }
    signal(SIGTERM, handle_signal);

    thread->id = clone(thread_start_wrapper, (char *)thread->stack + STACK_SIZE,
                       CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM,
                       thread);

    if (thread->id == -1) {
        perror("clone failed");
        free(thread->stack);
        return -1;
    }

    thread->status = 1;  // Hilo corriendo
    thread->parent_tid = syscall(SYS_gettid);
    return 0;
}

int CEthread_join(CEthread_t *thread, void **retval) {
    if (thread->status != 1) {
        fprintf(stderr, "Thread %d is not active or not created\n", thread->id);
        return -1;
    }

    // Esperar a que el hilo termine
    while (1) {
        int result = syscall(SYS_tgkill, getpid(), thread->id, 0);
        if (result == -1) {
            if (errno == ESRCH) {
                // El hilo ha terminado
                break;
            } else {
                perror("tgkill failed");
                return -1;
            }
        }
        usleep(1000);  // Esperar un milisegundo antes de verificar de nuevo
    }

    if (retval != NULL) {
        *retval = thread->retval;
    }

    free(thread->stack);
    thread->status = 0;
    return 0;
}


int CEthread_end(CEthread_t *thread) {
    if (thread->status != 1) {
        return -1;  // El hilo no está activo
    }

    // Enviar señal de cancelación (SIGTERM para este caso)
    if (syscall(SYS_tgkill, getpid(), thread->id, SIGTERM) != 0) {
        perror("tgkill failed");
        return -1;
    }

    // Esperar a que el hilo termine
    while (1) {
        // Verificar si el hilo sigue activo
        int result = syscall(SYS_tgkill, getpid(), thread->id, 0);
        if (result == -1) {
            if (errno == ESRCH) {
                // El hilo ha terminado
                break;
            } else {
                perror("tgkill failed");
                return -1;
            }
        }
        usleep(1000);  // Esperar un milisegundo antes de verificar nuevamente
    }

    // Limpiar recursos asociados
    if (thread->stack) {
        free(thread->stack);
        thread->stack = NULL; // Evitar punteros colgantes
    }
    thread->status = 0; // Marcar como terminado
    thread->retval = NULL; // Asegurarse de que el valor de retorno no sea utilizado
    return 0;
}

void CEthread_sleep(int segundos) {
    usleep(segundos * 1000000); // Convertir segundos a microsegundos
}


// Inicializar el mutex
int CEmutex_init(CEmutex_t *mutex) {
    if (sem_init(&mutex->semaphore, 0, 1) != 0) {
        perror("sem_init failed");
        return -1;
    }
    return 0;
}

// Bloquear el mutex
int CEmutex_lock(CEmutex_t *mutex) {
    if (sem_wait(&mutex->semaphore) != 0) {
        perror("sem_wait failed");
        return -1;
    }
    return 0;
}

// Desbloquear el mutex
int CEmutex_unlock(CEmutex_t *mutex) {
    if (sem_post(&mutex->semaphore) != 0) {
        perror("sem_post failed");
        return -1;
    }
    return 0;
}

// Destruir el mutex
int CEmutex_destroy(CEmutex_t *mutex) {
    if (sem_destroy(&mutex->semaphore) != 0) {
        perror("sem_destroy failed");
        return -1;
    }
    return 0;
}

