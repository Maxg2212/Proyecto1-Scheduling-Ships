//
// Created by ejbm0110 on 9/28/24.
//

#include "schedulers.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// Variable global para conocer el largo del canal
int channel = 0;

/**
 * Funcion que mueve los barcos modificando su posicion.
 * @param node Nodo que contiene la informacion del barco.
 * @param direction Indica si va de izquierda a derecha o de derecha a izquierda.
 * @author Eduardo Bolivar Minguet
 */
void update_position(struct Node* node, int direction) {
    // Update the boat position based on direction
    if (direction > 0) {
        node->boat_position += channel / node->burst_time;
    } else {
        node->boat_position -= channel / node->burst_time;
    }
}

/**
 * Funcion que ejecuta hilos uno tras otro. Para los algoritmos FCFS, Prioridad, SJF y EDF.
 * @param arg Nodo que funciona como PCB.
 * @return Estado de ejecucion
 * @author Eduardo Bolivar Minguet
 */
int thread_func(void* arg) {
    struct Node* node = (struct Node*)arg;
    int const direction = (node->boat_position == 0) ? 1 : -1;
    while ((direction == 1 && node->boat_position <= channel) || (direction == -1 && node->boat_position >= 0)) {
        update_position(node, direction);
    }
    printf("Hilo: %d - Bote: %s - Cola: %s\n", node->pid, node->boat_type, (direction == 1) ? "Izquierda" : "Derecha");
    return 0;
}

// Variables compartidas para la ejecucion de Round Robin
int turn, base, currently_executing;
double quantum1;
CEthread_mutex_t* mutex;

/**
 * Es el modelo de la funcion adaptado a la calendarizacion Round Robin.
 * Utiliza mutex y conditions para turnar a los hilos que acceden a la funcion y les da permiso para ejecutar durante
 * un quantum definido.
 * @param arg Nodo que funciona como PCB del hilo.
 * @return Estado de ejecucion
 * @author Eduardo Bolivar Minguet
 */
int thread_func_rr(void* arg) {
    struct Node* node = (struct Node*)arg;
    int const direction = (node->boat_position == 0) ? 1 : -1;
    while ((direction == 1 && node->boat_position <= channel) || (direction == -1 && node->boat_position >= 0)) {

        // Lock the mutex
        CEmutex_trylock(mutex);

        // Check shared resource
        if (turn == node->pid) {
            clock_t start = clock();
            while (quantum1 > (double) (clock() - start) / CLOCKS_PER_SEC) {
                update_position(node, direction);
            }

            // Modify shared resource
            turn = (turn + 1) % currently_executing + base + 1;
        }

        // Unlock mutex
        CEmutex_unlock(mutex);
    }
    printf("Hilo: %d - Bote: %s - Cola: %s\n", node->pid, node->boat_type, (direction == 1) ? "Izquierda" : "Derecha");
    return 0;
}

/**
 * Calendarizacion que inicia todos los hilos a la vez.
 * La funcion thread_func_rr es la encargada de turnar la ejecucion de los hilos
 * @param head Puntero a la primera posicion de la cola de hilos
 * @param W Es el parametro W de cuantos barcos pasan.
 * @param swapTime Es el tiempo de cambio del letrero.
 * @param local_quantum Es el quantum que va a utilizar.
 * @param length Es el largo del canal
 * @author Eduardo Bolivar Minguet
 */
void round_robin(struct Node** head, int const W, double const swapTime, double local_quantum, int length) {
    // Initiliaze mutex
    CEmutex_init(&mutex, NULL);

    // Initialize RR required information
    channel = length;
    turn = (*head)->pid;
    base = turn - 1;
    quantum1 = local_quantum;

    struct Node* current = *head; // Use a separate pointer to traverse the list

    // If W != 0, then the channel flow is Equidad
    if (W != 0) {

        // The executing threads will be given by W only if W is less than the length of queue
        currently_executing = get_length(*head) >= W ? W : get_length(*head);

        int tmpW = W;

        while (tmpW > 0 && current != nullptr) {
            CEthread_create(&current->process, nullptr, thread_func_rr, current);
            current = current->next; // Move to the next node
            tmpW--;
        }

        tmpW = W;
        current = *head;

        while (tmpW > 0 && current != nullptr) {
            CEthread_join(current->process);
            struct Node* next = current->next;
            remove_from_queue(&current);
            current = next;
            tmpW--;
        }
    }
    // If SwapTime != 0, then channel flow is Letrero
    else if (swapTime != 0) {

        // Set the variable to accumulate executing threads during Letrero time is active
        currently_executing = 0;

        // Start the timer
        clock_t const sign_start = clock();

        // Set how many
        while (swapTime > (double) (clock() - sign_start) / CLOCKS_PER_SEC) {
            currently_executing++;
        }

        // For each thread created, do the corresponding join function.
        int tmpX = currently_executing;
        while (tmpX > 0 && current != nullptr) {
            CEthread_create(&current->process, nullptr, thread_func_rr, current);
            current = current->next;
            tmpX--;
        }

        tmpX = currently_executing;
        current = *head;

        while (tmpX > 0 && current != nullptr) {
            CEthread_join(current->process);
            struct Node* next = current->next;
            remove_from_queue(&current);
            current = next;
            tmpX--;
        }
    }
    // If W and SwapTime are 0, then the flow is Tico
    else {

    }

    *head = current;
    CEmutex_destroy(mutex);
}

/**
 * Intercambia los atributos de los nodos para ordenamiento.
 * @param i El nodo actual
 * @param j El nodo siguiente
 * @author Max Garro Mora
 */
void swapAttributes(struct Node* i, struct Node* j) {
    char temp_boat_type[10];

    const int temp_pid = i->pid;
    i->pid = j->pid;
    j->pid = temp_pid;

    const int temp_priority = i->priority;
    i->priority = j->priority;
    j->priority = temp_priority;

    const double temp_position = i->boat_position;
    i->boat_position = j->boat_position;
    j->boat_position = temp_position;

    strcpy(temp_boat_type, i->boat_type);
    strcpy(i->boat_type, j->boat_type);
    strcpy(j->boat_type, temp_boat_type);

    const double temp_burst = i->burst_time;
    i->burst_time = j->burst_time;
    j->burst_time = temp_burst;

    CEthread_t* temp_process = i->process;
    i->process = j->process;
    j->process = temp_process;
}

/**
 * Ordena la cola de mayor a menor prioridad.
 * @param head Puntero a la cola.
 * @return Puntero de la cola ordenada.
 * @author Max Garro Mora
 */
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

/**
 * Calendarizador por prioridad. Ejecuta los hilos por orden de prioridad.
 * @param head Puntero a la cola de hilos.
 * @param W Parametro W que indica cuantos barcos pasan.
 * @param swapTime Tiempo de cambio del letrero.
 * @param length Largo del canal
 * @author Eduardo Bolivar Minguet
 */
void priority(struct Node** head, int const W, double const swapTime, int const length) {
    // Ordena la cola por prioridad
    *head = sort_by_priority(*head);

    // Ejecuta un FCFS de la cola ordenada.
    first_come_first_served(head, W, swapTime, length);
}

/**
 * Ordena la cola de menor a mayor tiempo de duracion del hilo.
 * @param head Puntero a la cola.
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
 * Calendarización SJF. Ejecuta los hilos con menor tiempo primero.
 * @param head Puntero a la cola de hilos.
 * @author Eduardo Bolivar Minguet
 */
void shortest_job_first(struct Node** head, int const W, double const swapTime, int const length) {

    // Ordena la cola por duracion de hilo
    *head = sort_by_burst_time(*head);

    // Ejecuta un FCFS a la cola ordenada
    first_come_first_served(head, W, swapTime, length);
}

/**
 * Calendarizacion FCFS. Ejecuta la cola de hilos dada.
 * @param head Puntero a la cola de hilos.
 * @author Eduardo Bolivar Minguet
 */
void first_come_first_served(struct Node** head, int W, double swapTime, int const length) {
    channel = length;

    // Ejecuta W hilos para algoritmo Equidad
    if (W != 0) {
        while (W > 0 && *head != nullptr) {
            CEthread_create(&(*head)->process, nullptr, thread_func, *head);
            CEthread_join((*head)->process);
            remove_from_queue(head);
            W--;
        }
    }
    // Ejecuta la cantidad de hilos posible dentro del tiempo del letrero en el algoritmo Letrero
    else if (swapTime != 0) {
        clock_t letrero_start = clock();
        while (swapTime > (double) (clock() - letrero_start) / CLOCKS_PER_SEC && *head != nullptr) {
            CEthread_create(&(*head)->process, nullptr, thread_func, *head);
            CEthread_join((*head)->process);
            remove_from_queue(head);
        }
    }
    // Algoritmo Tico
    else {

    }
}

/**
 * Ordena la cola de hilos de forma que se ejecuten primero las patrullas.
 * @param head Puntero a la cola de hilos.
 * @return Puntero a la cola ordenada
 * @author Eduardo Bolivar Minguet
 */
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

/**
 * Calendarizador EDF. Ejecuta primero las patrullas ya que son los unicos procesos con deadline asociado.
 * @param head Puntero a la cola de hilos.
 * @param numOfPatrols Cantidad de patrullas que debe dejar pasar.
 * @param length Largo del canal
 */
void earliest_deadline_first(struct Node** head, int const numOfPatrols, int const length) {
    // Ordena patrullas primero
    *head = sort_by_patrols(*head);

    // Aplica un FCFS
    first_come_first_served(head, numOfPatrols, 0, length);
}