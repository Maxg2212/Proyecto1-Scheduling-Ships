//
// Created by ejbm0110 on 9/30/24.
//

#include <pthread.h>
#include <time.h>
#include "channel_algorithms.h"
#include <stdio.h>

void* identify(void* arg) {
    struct Node* currentNode = (struct Node*)arg;
    printf("Soy el hilo %d, soy un barco %s y mi tiempo de duracion es: %.5f\n", currentNode->pid, currentNode->boat_type, currentNode->burst_time);
    return nullptr;
}

/**
 * PROCESS_QUEUE
 *
 * Ejecuta un elemento de la cola de hilos
 *
 * @param queue La cola de hilos
 * @return La cola sin el elemento recien ejecutado.
 * @author Eduardo Bolivar Minguet
 */
void process_queue(struct Node** queue) {
    struct Node* currentNode = *queue;
    pthread_create(&currentNode->process, NULL, identify, currentNode);
    pthread_join(currentNode->process, NULL);
    remove_from_queue(queue);
}

/**
 * EQUITY
 *
 * Ejecuta las filas de barcos con el algoritmo Equidad.
 * Cambia de lado cuando pasan W barcos.
 *
 * @param W La cantidad de hilos que ejecutan por iteracion
 * @param r_queue Cola de barcos derecha
 * @param l_queue Cola de barcos izquierda
 * @author Eduardo Bolivar Minguet
 */
void equity(int const W, struct Node** r_queue, struct Node** l_queue) {
    int modW = W;
    int flag = 1;
    while (*r_queue != nullptr || *l_queue != nullptr) {
        if (modW == 0) {
            flag = (flag + 1) % 2;
            modW = W;
        }
        if (flag && *r_queue != nullptr) {
            process_queue(r_queue);
        } else if (!flag && *l_queue != nullptr) {
            process_queue(l_queue);
        }
        modW--;
    }
}

/**
 * SIGNBOARD
 *
 * Ejecuta las filas de barcos con el algoritmo Letrero.
 * Cambia de lado cuando que se llega al tiempo definido.
 *
 * @param swap_time El tiempo de cambio del letrero.
 * @param r_queue Cola de barcos derecha
 * @param l_queue Cola de barcos izquierda.
 * @author Eduardo Bolivar Minguet
 */
void signboard(double const swap_time, struct Node** r_queue, struct Node** l_queue) {
    int flag = 1;
    clock_t start = clock();
    while (*r_queue != nullptr || *l_queue != nullptr) {
        if (swap_time <= (double) (clock() - start) / CLOCKS_PER_SEC) {
            flag = (flag + 1) % 2;
            start = clock();
        }
        if (flag && *r_queue != nullptr) {
            process_queue(r_queue);
        } else if (!flag && *l_queue != nullptr) {
            process_queue(l_queue);
        }
    }
}

/**
 * TICO
 *
 * No controla flujo, solo deja pasar barcos de ambos lados.
 *
 * @param r_queue Cola de barcos derecha.
 * @param l_queue COla de barcos izquierda.
 * @author Eduardo Bolivar Minguet
 */
void tico(struct Node** r_queue, struct Node** l_queue) {
    while (r_queue != nullptr || l_queue != nullptr) {
        process_queue(r_queue);
        process_queue(l_queue);
    }
}
