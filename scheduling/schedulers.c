//
// Created by ejbm0110 on 9/28/24.
//

#include "schedulers.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int channel = 0;

void update_position(struct Node* node, int direction) {
    // Update the boat position based on direction
    if (direction > 0) {
        node->boat_position += channel / node->burst_time;
    } else {
        node->boat_position -= channel / node->burst_time;
    }
    printf("Hilo: %d - Bote: %s - Posicion: %f\n", node->pid, node->boat_type, node->boat_position);
}

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
    struct Node* node = (struct Node*)arg;
    int const direction = (node->boat_position == 0) ? 1 : -1;
    while ((direction == 1 && node->boat_position <= channel) || (direction == -1 && node->boat_position >= 0)) {
        update_position(node, direction);
    }
    return nullptr;
}

int turn;   // Se define que hilo tiene el turno de ejecucion, inicia en 0, el primero en cola.
int base;
double root_quantum; // Tiempo de ejecucion de cada hilo en su turno
int queue_length;   // Tamano de la cola de hilos
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex protege condiciones de carrera.
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
    struct Node* node = (struct Node*)arg;
    int const direction = (node->boat_position == 0) ? 1 : -1;
    while ((direction == 1 && node->boat_position <= channel) || (direction == -1 && node->boat_position >= 0)) {
        clock_t const quantum_start = clock();
        while (turn == node->pid && root_quantum > (double) (clock() - quantum_start) / CLOCKS_PER_SEC) {
            update_position(node, direction);
            if ((direction == 1 && node->boat_position > channel) || (direction == -1 && node->boat_position < 0)) {
                break;
            }
        }
        pthread_mutex_lock(&mutex);
        turn++;
        pthread_mutex_unlock(&mutex);
        if (turn > base + queue_length) {
            pthread_mutex_lock(&mutex);
            turn = base + 1;
            pthread_mutex_unlock(&mutex);
        }
    }
    return nullptr;
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
void round_robin(struct Node** head, int const offset, double quantum, int length) {
    int flag = ((*head)->boat_position == 0) ? 0 : 1;
    channel = length;
    root_quantum = quantum;
    queue_length = get_length(*head);
    if (flag) {
        turn = offset + 1;
        base = offset;
    } else {
        turn = 1;
        base = 0;
    }
    struct Node* current = *head;
    while (current != nullptr) {
        pthread_create(&current->process, nullptr, thread_func_rr, current);
        current = current->next;
    }
    while (*head != nullptr) {
        pthread_join((*head)->process, nullptr);
        remove_from_queue(head);
    }
}

void swapAttributes(struct Node* i, struct Node* j) {
    char temp_boat_type[10];

    const int temp_pid = i->pid;
    i->pid = j->pid;
    j->pid = temp_pid;

    const int temp_priority = i->priority;
    i->priority = j->priority;
    j->priority = temp_priority;

    const int temp_position = i->boat_position;
    i->boat_position = j->boat_position;
    j->boat_position = temp_position;

    strcpy(temp_boat_type, i->boat_type);
    strcpy(i->boat_type, j->boat_type);
    strcpy(j->boat_type, temp_boat_type);

    const double temp_burst = i->burst_time;
    i->burst_time = j->burst_time;
    j->burst_time = temp_burst;

    const pthread_t temp_process = i->process;
    i->process = j->process;
    j->process = temp_process;
}

struct Node* sort_by_priority(struct Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head; // No se necesita ordenar si hay uno o ningún nodo
    }
    struct Node *i, *j;

    for (i = head; i != nullptr; i = i->next) {
        for (j = i->next; j != nullptr; j = j->next) {
            if (i->priority > j->priority) {
                // Intercambiar valores
                swapAttributes(i, j);
            }
        }
    }
    return head;
}

void priority(struct Node** head, int length) {
    *head = sort_by_priority(*head);
    first_come_first_served(head, length);
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
    struct Node *i, *j;

    for (i = head; i != nullptr; i = i->next) {
        for (j = i->next; j != nullptr; j = j->next) {
            if (i->burst_time > j->burst_time) {
                // Intercambiar valores
                swapAttributes(i, j);
            }
        }
    }
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
void shortest_job_first(struct Node** head, int length) {
    *head = sort_by_burst_time(*head);
    first_come_first_served(head, length);
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
void first_come_first_served(struct Node** head, int  length) {
    channel = length;
    while (*head != nullptr && get_length(*head) > 0) {
        pthread_create(&(*head)->process, nullptr, thread_func, *head);
        pthread_join((*head)->process, nullptr);
        remove_from_queue(head);
    }
}

struct Node* sort_by_patrols(struct Node* head) {
    if (head == nullptr || head->next == nullptr) {
        return head; // No se necesita ordenar si hay uno o ningún nodo
    }
    struct Node *i, *j;

    for (i = head; i != nullptr; i = i->next) {
        for (j = i->next; j != nullptr; j = j->next) {
            if (strcmp(j->boat_type, "Patrulla") == 0 && strcmp(i->boat_type, "Patrulla") != 0) {
                // Intercambiar valores
                swapAttributes(i, j);
            }
        }
    }
    return head;
}

void earliest_deadline_first(struct Node** head, int length) {
    *head = sort_by_patrols(*head);
    first_come_first_served(head, length);
}