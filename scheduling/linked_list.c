//
// Created by ejbm0110 on 9/27/24.
//

#include<stdlib.h>
#include"linked_list.h"

#include <string.h>

/**
 * Crea un nodo en la cola para un nuevo hilo (barco)
 * @param id ID de hilo
 * @param burst_time Tiempo del hilo
 * @param priority Prioridad del hilo
 * @param boat_type Tipo de barco
 * @param position Posicion inicial del barco
 * @param process hilo
 * @return Nuevo nodo
 * @author Eduardo Bolivar Minguet
 */
struct Node* create_node(int const id, double const burst_time, int const priority, char boat_type[10], double const position, CEthread_t* process) {
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->pid = id;
    new_node->burst_time = burst_time;
    new_node->priority = priority;
    strcpy(new_node->boat_type, boat_type);
    new_node->boat_position = position;;
    new_node->process = process;
    new_node->next = nullptr;
    return new_node;
}

/**
 * Agrega un nodo a la cola
 * @param queue_ref Puntero a la cola.
 * @param id ID de hilo
 * @param burst_time Tiempo del hilo
 * @param priority Prioridad del hilo
 * @param boat_type Tipo de barco
 * @param position Es la coordenada inicial del barco.
 * @param process Hilo
 * @author Eduardo Bolivar
 */
void add_to_queue(struct Node** queue_ref, int const id, double const burst_time, int const priority, char* boat_type, double position, CEthread_t* process) {
    if (*queue_ref == nullptr) {
        *queue_ref = create_node(id, burst_time, priority, boat_type, position, process);
    } else {
        struct Node* current = *queue_ref;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = create_node(id, burst_time, priority, boat_type, position, process);
    }
}

/**
 * Elimina el primer nodo de la cola de hilos.
 * @param queue_ref Puntero a la primera posicion de la cola
 * @author Eduardo Bolivar
 */
void remove_from_queue(struct Node** queue_ref) {
    if (*queue_ref != nullptr) {
        struct Node* temp = *queue_ref;
        *queue_ref = temp->next;
        free(temp);
    }
}

/**
 * Calcula el tamano de la cola de hilos.
 * @param queue_ref Puntero a la cola
 * @return Tamano de cola
 * @author Eduardo Bolivar Minguet
 */
int get_length(struct Node* queue_ref) {
    int length = 0;
    while (queue_ref != nullptr) {
        length++;
        queue_ref = queue_ref->next;
    }
    return length;
}
