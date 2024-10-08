//
// Created by ejbm0110 on 9/30/24.
//

#include <pthread.h>
#include <time.h>
#include "channel_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../scheduling/schedulers.h"

/**
 * PROCESS_QUEUE
 *
 * Ejecuta un elemento de la cola de hilos
 *
 * @param queue La cola de hilos
 * @return La cola sin el elemento recien ejecutado.
 * @author Eduardo Bolivar Minguet
 */
void process_queues(struct Node** r_queue, struct Node** l_queue, int W, double swapTime, double rr_quantum, char scheduler[10], int clength) {
    if (strcmp(scheduler, "FCFS") == 0) {
        while (*r_queue != nullptr || *l_queue != nullptr) {
            first_come_first_served(l_queue, W, swapTime, clength);
            first_come_first_served(r_queue, W, swapTime, clength);
        }
    } else if (strcmp(scheduler, "RR") == 0) {
        while (*r_queue != nullptr || *l_queue != nullptr) {
            round_robin(l_queue, W, swapTime, rr_quantum, clength);
            round_robin(r_queue, W, swapTime, rr_quantum, clength);
        }
    } else if (strcmp(scheduler, "SJF") == 0) {
        while (*r_queue != nullptr || *l_queue != nullptr) {
            shortest_job_first(l_queue, W, swapTime, clength);
            shortest_job_first(r_queue, W, swapTime, clength);
        }
    } else if (strcmp(scheduler, "Prioridad") == 0) {
        while (*r_queue != nullptr || *l_queue != nullptr) {
            priority(l_queue, W, swapTime, clength);
            priority(r_queue, W, swapTime, clength);
        }
    } else {
        perror("Unknown scheduler");
        exit(1);
    }
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
void equity(int const W, struct Node** r_queue, struct Node** l_queue, char scheduler[20], double rr_quantum, int clength) {
    process_queues(r_queue, l_queue, W, 0, rr_quantum, scheduler, clength);
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
void signboard(double const swap_time, struct Node** r_queue, struct Node** l_queue, char scheduler[20], double rr_quantum, int clength) {
    process_queues(r_queue, l_queue, 0, swap_time, rr_quantum, scheduler, clength);
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
void tico(struct Node** r_queue, struct Node** l_queue, char scheduler[20], double rr_quantum, int clength) {
    while (r_queue != nullptr || l_queue != nullptr) {
        //process_queue(r_queue);
        //process_queue(l_queue);
    }
}
