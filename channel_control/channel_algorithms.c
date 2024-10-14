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
 * Llama al calendarizador configurado a ejecutar ambas colas de hilos.
 * @param r_queue Cola derecha de hilos
 * @param l_queue Cola izquierda de hilos
 * @param W Parametro W de Equidad
 * @param swapTime Tiempo de cambio del letrero
 * @param rr_quantum Quantum configurado para Round Robin
 * @param scheduler Nombre del calendarizador
 * @param clength Largo del canal
 * @return La cola sin el elemento recien ejecutado.
 * @author Eduardo Bolivar Minguet
 */
void process_queues(struct Node** r_queue, struct Node** l_queue, int W, double swapTime, char scheduler[10]) {

    // First Come First Served
    if (strcmp(scheduler, "FCFS") == 0) {
        if (*l_queue != nullptr && *r_queue != nullptr) {
            first_come_first_served(l_queue, r_queue, W, swapTime);
        }
    }

    // Shortest Job First
    else if (strcmp(scheduler, "SJF") == 0) {
        if (*l_queue != nullptr && *r_queue != nullptr) {
            shortest_job_first(l_queue, r_queue, W, swapTime);
        }
    }

    // Priority
    else if (strcmp(scheduler, "Prioridad") == 0) {
        if (*l_queue != nullptr && *r_queue != nullptr) {
            priority(l_queue, r_queue, W, swapTime);
        }
    } else {
        perror("Unknown scheduler");
        exit(1);
    }
}

/**
 * Procesa las colas con el algoritmo Equidad
 * @param W La cantidad de barcos que pasan de cada lado.
 * @param r_queue Cola de barcos derecha
 * @param l_queue Cola de barcos izquierda
 * @param scheduler Calendarizador escogido
 * @param rr_quantum Quantum para Round Robin
 * @param clength Largo del canal
 * @author Eduardo Bolivar Minguet
 */
void equity(int const W, struct Node** r_queue, struct Node** l_queue, char scheduler[20]) {
    // Pasa un swapTime igual a 0 ya que este algoritmo no lo ocupa
    process_queues(r_queue, l_queue, W, 0, scheduler);
}

/**
 * Procesa las colas con el algoritmo del Letrero.
 * @param swap_time El tiempo de cambio del letrero.
 * @param r_queue Cola de barcos derecha
 * @param l_queue Cola de barcos izquierda.
 * @param scheduler Calendarizador escogido
 * @param rr_quantum Quantum para Round Robin
 * @param clength Largo del canal
 * @author Eduardo Bolivar Minguet
 */
void signboard(double const swap_time, struct Node** r_queue, struct Node** l_queue, char scheduler[20]) {
    // Pasa un W igual a 0 ya que este no lo ocupa
    process_queues(r_queue, l_queue, 0, swap_time, scheduler);
}

/**
 * Procesa las colas sin control de flujo
 * @param r_queue Cola de barcos derecha.
 * @param l_queue COla de barcos izquierda.
 * @param scheduler Calendarizador escogido
 * @param rr_quantum Quantum para ROund Robin
 * @param clength Largo del canal
 * @author Eduardo Bolivar Minguet
 */
void tico(struct Node** r_queue, struct Node** l_queue, char scheduler[20]) {
    // Pasa W y swapTime iguales a 0 ya que no controla el flujo.
    process_queues(r_queue, l_queue, 0, 0, scheduler);
}
