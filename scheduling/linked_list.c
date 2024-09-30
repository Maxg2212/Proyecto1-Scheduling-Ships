//
// Created by ejbm0110 on 9/27/24.
//

#include<stdlib.h>
#include"linked_list.h"

/**
 * CREATE_NODE
 *
 * Crea un nodo en la cola para un nuevo hilo
 * @param id ID de hilo
 * @param process hilo
 * @return Nodo
 * @author Eduardo Bolivar Minguet
 */
struct Node* create_node(int id, pthread_t const process, double burst_time, int priority) {
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->pid = id;
    new_node->process = process;
    new_node->burst_time = burst_time;
    new_node->priority = priority;
    new_node->next = nullptr;
    return new_node;
}

/**
 * ADD_TO_QUEUE
 *
 * Agrega un nodo a la cola
 * @param head Puntero a la primera posicion de la cola.
 * @param id ID de hilo
 * @param process Hilo
 * @return Puntero a la cola ya con el nodo agregado
 * @author Eduardo Bolivar
 */
struct Node* add_to_queue(struct Node* head, int const id, pthread_t const process, double burst_time, int priority) {
    if (head == nullptr) {
        return create_node(id, process, burst_time, priority);
    }
    struct Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = create_node(id, process, burst_time, priority);
    return head;
}

/**
 * REMOVE_FROM_QUEUE
 *
 * Elimina el primer nodo de la cola de hilos.
 *
 * @param head Puntero a la primera posicion de la cola
 * @return Puntero a la cola con el nodo ya eliminado
 */
struct Node* remove_from_queue(struct Node* head) {
    struct Node* temp = head;
    head = head->next;
    free(temp);
    return head;
}

/**
 * GET_LENGTH
 *
 * Calcula el tamano de la cola de hilos
 *
 * @param head Puntero a la cola
 * @return Tamano de cola
 * @author Eduardo Bolivar Minguet
 */
int get_length(struct Node* head) {
    int length = 0;
    while (head != nullptr) {
        length++;
        head = head->next;
    }
    return length;
}
