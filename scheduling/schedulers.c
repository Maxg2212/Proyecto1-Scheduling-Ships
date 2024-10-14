//
// Created by ejbm0110 on 9/28/24.
//

#include "schedulers.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "../HW/arduino-serial-lib.h"

CEthread_mutex_t *pos_mutex, *w_mutex;
int mode = 0;
int W = 0;
double swap = 0;

/**
 * Funcion que ejecuta hilos uno tras otro. Para los algoritmos FCFS, Prioridad, SJF y EDF.
 * @param arg Nodo que funciona como info del barco.
 * @return Estado de ejecucion
 * @author Eduardo Bolivar Minguet
 */
void move_boat(void* arg) {
    struct Node* this_boat = arg;

    int const border1 = 900 - this_boat->channel / 2;
    int const border2 = 900 + this_boat->channel / 2;

    int direction = (this_boat->x < 900) ? 1 : -1;
    while (direction == 1 && this_boat->x < 900 + this_boat->channel / 2) {

        if (820 - this_boat->channel / 2 < this_boat->x && this_boat->x < 900 + this_boat->channel / 2) {
            this_boat->y = 295;
        } else {
            this_boat->y = 230;
        }

        if (this_boat->prev == nullptr || (this_boat->prev != nullptr && this_boat->prev->x - this_boat->x + this_boat->speed > 100)) {
            if (W > 0 || this_boat->x + 100 < border1 || (this_boat->x + 100 > border1 && W == 0)) {
                CEmutex_trylock(pos_mutex);
                this_boat->x += direction * this_boat->speed;
                CEmutex_unlock(pos_mutex);

                if (this_boat->x + 100 == border1) {
                    CEmutex_trylock(w_mutex);
                    W--;
                    CEmutex_unlock(w_mutex);
                }
            }
        }

        SDL_Delay(16);
    }
    while (direction == -1 && this_boat->x > 820 - this_boat->channel / 2) {
        if (820 - this_boat->channel / 2 < this_boat->x && this_boat->x < 900 + this_boat->channel / 2) {
            this_boat->y = 295;
        } else {
            this_boat->y = 370;
        }

        if (this_boat->prev == nullptr || (this_boat->prev != nullptr && this_boat->x - this_boat->prev->x - this_boat->speed > 100)) {
            if (W > 0 || this_boat->x > border2 || (this_boat->x < border2 && W == 0)) {
                CEmutex_trylock(pos_mutex);
                this_boat->x += direction * this_boat->speed;
                CEmutex_unlock(pos_mutex);

                if (this_boat->x == border2) {
                    CEmutex_trylock(w_mutex);
                    W--;
                    CEmutex_unlock(w_mutex);
                }
            }
        }

        SDL_Delay(16);
    }
}

void move_patrol(void* arg) {
    struct Node* this_boat = arg;
    int direction = (this_boat->x < 900) ? 1 : -1;
    while (direction == 1 && this_boat->x < 900 + this_boat->channel / 2) {
        if (820 - this_boat->channel / 2 < this_boat->x && this_boat->x < 900 + this_boat->channel / 2) {
            this_boat->y = 295;
        } else {
            this_boat->y = 230;
        }
        this_boat->x += direction * 8;
        SDL_Delay(16);
    }
    while (direction == -1 && this_boat->x > 820 - this_boat->channel / 2) {
        if (820 - this_boat->channel / 2 < this_boat->x && this_boat->x < 900 + this_boat->channel / 2) {
            this_boat->y = 295;
        } else {
            this_boat->y = 370;
        }
        this_boat->x += direction * 8;
        SDL_Delay(16);
    }
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

    strcpy(temp_boat_type, i->boat_type);
    strcpy(i->boat_type, j->boat_type);
    strcpy(j->boat_type, temp_boat_type);

    const int temp_speed = i->speed;
    i->speed = j->speed;
    j->speed = temp_speed;
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
void priority(struct Node** head, int const W, double const swapTime) {
    // Ordena la cola por prioridad
    *head = sort_by_priority(*head);

    // Ejecuta un FCFS de la cola ordenada.
    first_come_first_served(head, W, swapTime);
}

/**
 * SORT_BY_BURST_TIME
 *
 * Ordena la lista enlazada basada en el tiempo de ráfaga (burst_time) usando el algoritmo de burbuja.
 * Se agrega mutex para evitar condiciones de carrera al ordenar.
 *
 * @param head Puntero a la primera posición de la cola de hilos
>>>>>>> GUI
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
            if (i->speed < j->speed) {
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
void shortest_job_first(struct Node** head, int const W, double const swapTime) {

    // Ordena la cola por duracion de hilo
    *head = sort_by_burst_time(*head);

    // Ejecuta un FCFS a la cola ordenada
    first_come_first_served(head, W, swapTime);
}

/**
 * Calendarizacion FCFS. Ejecuta la cola de hilos dada.
 * @param head Puntero a la cola de hilos.
 * @author Eduardo Bolivar Minguet
 */
void first_come_first_served(struct Node** head, int local_W, double swapTime) {
    W = local_W;
    swap = swapTime;
    mode = (W != 0) ? 1 : (swapTime != 0) ? 2 : 3;
    // Ejecuta W hilos para algoritmo Equidad
    for (struct Node* current = *head; current != nullptr; current = current->next) {
        CEthread_create(&current->t, nullptr, move_boat, current);
    }
    for (struct Node* current = *head; current != nullptr; current = current->next) {
        CEthread_join((*head)->t);
        remove_from_queue(head);
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
void earliest_deadline_first(struct Node** head, int numOfPatrols) {
    // Ordena patrullas primero
    *head = sort_by_patrols(*head);

    int numOfPatrols2 = numOfPatrols;

    // Aplica un FCFS
    for (struct Node* current = *head; numOfPatrols > 0 && current != nullptr; current = current->next) {
        CEthread_create(&current->t, nullptr, move_patrol, current);
        numOfPatrols--;
    }
    while (numOfPatrols2 > 0 && *head != nullptr) {
        CEthread_join((*head)->t);
        remove_from_queue(head);
        numOfPatrols2--;
    }
}
void init_izquierda()
{
    const char* mes = "a";
    int dev = serialport_init("/dev/ttyACM0", 9600);
    //usleep(2000);
    serialport_write(dev, mes);
}

void init_derecha()
{
    const char* mes = "b";
    int dev = serialport_init("/dev/ttyACM0", 9600);
    //usleep(2000);
    serialport_write(dev, mes);
}

void init_mutex() {
    CEmutex_init(&pos_mutex, nullptr);
    CEmutex_init(&w_mutex, nullptr);
}