//
// Created by ejbm0110 on 9/28/24.
//

#include "schedulers.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>

/**
 * THREAD_FUNC
 *
 * Es el modelo para ejecutar hilos uno tras otro. Para los algoritmos FCFS, Prioridad y SJF.
 *
 * @param arg ID del hilo que llama a la funcion.
 * @return puntero a nulo
 * @author Eduardo Bolivar Minguet
 */
void* thread_func(void* arg) {
    int count = 0;
    while (count < 1000) {
        printf("Thread %d - Counter: %d\n", *(int*)arg, count++);
    }
    return nullptr;
}

int turn = 0;   // Se define que hilo tiene el turno de ejecucion, inicia en 0, el primero en cola.
double quantum; // Tiempo de ejecucion de cada hilo en su turno
int queue_length;   // Tamano de la cola de hilos
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex protege condiciones de carrera.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // Cond pone en espera a hilos y avisarles cuando sea su turno.

/**
 * THREAD_FUNC_RR
 *
 * Es el modelo de la funcion THREAD_FUNC adaptado a la calendarizacion Round Robin.
 * Utiliza mutex y conditions para turnar a los hilos que acceden a la funcion y les da permiso para ejecutar durante
 * un quantum definido.
 *
 * @param arg ID del hilo que llama a la funcion
 * @return puntero a nulo
 * @author Eduardo Bolivar Minguet
 */
void* thread_func_rr(void* arg) {
    int count = 0;
    while (count < 1000) {
        pthread_mutex_lock(&mutex);
        while (turn != *(int*)arg) {
            pthread_cond_wait(&cond, &mutex);   // Si no es su turno, el hilo espera
        }
        pthread_mutex_unlock(&mutex);
        clock_t inicio_quantum = clock();   // Se empieza a contar el quantum
        while (quantum >= (double) (clock() - inicio_quantum) / CLOCKS_PER_SEC) {
            printf("Thread %d - Counter: %d\n", *(int*)arg, count++);
            if (count >= 1000) {
                break;
            }
        }
        pthread_mutex_lock(&mutex);
        turn = (turn + 1) % queue_length;   // Le da el turno al siguiente hilo en cola
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    return nullptr;
}

/**
 * FIRST_COME_FIRST_SERVED
 *
 * Calendarizacion que atiende a cada hilo, uno por uno, hasta que se vacie la cola de hilos.
 * Cada hilo espera que termine el anterior.
 *
 * @param head Puntero a la primera posicion de la cola de hilos.
 * @author Eduardo Bolivar Minguet
 */
void first_come_first_served(struct Node* head) {
    while (head != nullptr) {
        pthread_create(&head->process, NULL, thread_func, &head->pid);
        pthread_join(head->process, NULL);
        head = remove_from_queue(head);
    }
}

/**
 * ROUND_ROBIN
 *
 * Calendarizacion que inicia todos los hilos a la vez.
 * La funcion THREAD_FUNC_RR es la encargada de turnar la ejecucion de los hilos
 *
 * @param head Puntero a la primera posicion de la cola de hilos
 * @author Eduardo Bolivar Minguet
 */
void round_robin(struct Node* head) {
    quantum = 0.005;
    queue_length = get_length(head);
    struct Node* current = head;
    while (current != nullptr) {
        pthread_create(&current->process, NULL, thread_func_rr, &current->pid);
        current = current->next;
    }
    while (head != nullptr) {
        pthread_join(head->process, NULL);
        head = remove_from_queue(head);
    }
}

/**
 * SORT_BY_BURST_TIME
 *
 * Ordena la lista enlazada basada en el tiempo de ráfaga (burst_time) usando el algoritmo de burbuja.
 * Se agrega mutex para evitar condiciones de carrera al ordenar.
 *
 * @param head Puntero a la primera posición de la cola de hilos
 * @return Puntero a la lista enlazada ordenada
 * @author Max Garro Mora
 */
struct Node* sort_by_burst_time(struct Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head; // No se necesita ordenar si hay uno o ningún nodo
    }

    pthread_mutex_lock(&mutex);  // Protege la lista enlazada durante la ordenación

    struct Node *i, *j;
    double temp_burst;
    int temp_pid, temp_priority;
    pthread_t temp_process;

    for (i = head; i != nullptr; i = i->next) {
        for (j = i->next; j != nullptr; j = j->next) {
            if (i->burst_time > j->burst_time) {
                // Intercambiar valores
                temp_burst = i->burst_time;
                i->burst_time = j->burst_time;
                j->burst_time = temp_burst;

                temp_pid = i->pid;
                i->pid = j->pid;
                j->pid = temp_pid;

                temp_priority = i->priority;
                i->priority = j->priority;
                j->priority = temp_priority;

                temp_process = i->process;
                i->process = j->process;
                j->process = temp_process;
            }
        }
    }

    pthread_mutex_unlock(&mutex);  // Libera el mutex después de ordenar

    return head;
}


/**
 * SHORTEST_JOB_FIRST
 *
 * Calendarización basada en SJF. Ordena la lista de hilos según su tiempo de ráfaga (burst_time)
 * y luego ejecuta cada hilo uno por uno, esperando a que termine el anterior.
 * Se usa mutex para proteger la cola de hilos.
 *
 * @param head Puntero a la primera posición de la cola de hilos.
 * @author Eduardo Bolivar Minguet
 */
void shortest_job_first(struct Node* head) {
    pthread_mutex_lock(&mutex);  // Protege el acceso a la lista enlazada mientras se ordena
    head = sort_by_burst_time(head);
    pthread_mutex_unlock(&mutex);  // Libera el mutex una vez que se ha ordenado

    // Ejecutar los hilos en el orden de menor a mayor burst_time
    while (head != nullptr) {
        pthread_create(&head->process, NULL, thread_func, &head->pid);
        pthread_join(head->process, NULL);

        pthread_mutex_lock(&mutex);  // Protege la lista enlazada al eliminar el nodo
        head = remove_from_queue(head);
        pthread_mutex_unlock(&mutex);  // Libera el mutex después de eliminar
    }
}