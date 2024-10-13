//
// Created by ejbm0110 on 9/27/24.
//

#include<stdlib.h>
#include"linked_list.h"

#include <string.h>

/**
 * Crea un nodo en la cola para un nuevo hilo (barco)
 * @param pid ID de hilo
 * @param burst_time Tiempo del hilo
 * @param priority Prioridad del hilo
 * @param boat_type Tipo de barco
 * @param x Posicion inicial del barco en X
 * @param y Posicion inicial del barco en Y
 * @param t hilo
 * @return Nuevo nodo
 * @author Eduardo Bolivar Minguet
 */
struct Node* create_node(int pid, int priority, char* boat_type, int speed, int x, int y, CEthread_t* t, int channel) {
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->pid = pid;
    new_node->priority = priority;
    strcpy(new_node->boat_type, boat_type);
    new_node->speed = speed;
    new_node->x = x;
    new_node->y = y;
    new_node->t = t;
    new_node->next = nullptr;
    new_node->channel = channel;
    return new_node;
}

/**
 * Agrega un nodo a la cola
 * @param queue_ref Puntero a la cola.
 * @param pid ID de hilo
 * @param burst_time Tiempo del hilo
 * @param priority Prioridad del hilo
 * @param boat_type Tipo de barco
 * @param x Posicion inicial del barco en X
 * @param y Posicion inicial del barco en Y
 * @param t hilo
 * @author Eduardo Bolivar
 */
void add_to_queue(struct Node** queue_ref, int pid, int priority, char* boat_type, int speed, int x, int y, CEthread_t* t, int channel) {
    if (*queue_ref == nullptr) {
        *queue_ref = create_node(pid, priority, boat_type, speed, x, y, t, channel);
    } else {
        struct Node* current = *queue_ref;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = create_node(pid, priority, boat_type, speed, x, y, t, channel);
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
int get_length(const struct Node* queue_ref) {
    int length = 0;
    while (queue_ref != nullptr) {
        length++;
        queue_ref = queue_ref->next;
    }
    return length;
}
