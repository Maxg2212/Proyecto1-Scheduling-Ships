#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "channel_control/channel_algorithms.h"
#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"
#include "GUI/guidef.h"
#include <string.h>
#include <time.h>

// ----------------- Configuration parameters -----------------------------------
char scheduler[20];
char flow[20];
int w, channelLength, boatSpeed, normalPriority, fishingPriority, patrolDeadline;
int normalBoatsRight, fishingBoatsRight, patrolBoatsRight;
int normalBoatsLeft, fishingBoatsLeft, patrolBoatsLeft;
double timeSwap, quantum;

int totalPatrolBoats, tmp1, tmp2;

struct Node* left_queue = nullptr;
struct Node* right_queue = nullptr;

int id_left = 1;
int id_right = 1;

// --------------------------------------------------------------------------------

/**
 * Genera un tipo de barco de entre los tres disponibles de forma aleatoria.
 * @return Cadena de caracteres con el nombre del barco.
 * @author Eduardo Bolivar Minguet
 */
const char* get_random_boat() {
    const char* boat_types[3] = {"Normal", "Pesquero", "Patrulla"};
    int random_index = rand() % 3;
    return boat_types[random_index];
}

/**
 * Detecta la interrupcion por teclado e invoca a la funcion para generar un barco random.
 * @author Eduardo Bolivar Minguet
 * @see get_random_boat
 */
void add_boat_by_key() {
    CEthread_t* thread;
    int side = (rand() % 2) + 1;
    switch (side) {
        case 1:
            add_to_queue(&left_queue, id_left++, 4, 0, get_random_boat(), 0, thread);
            left_queue->boat_position = 0;
            break;
        case 2:
            add_to_queue(&right_queue, id_right++, 4, 0, get_random_boat(), channelLength, thread);
            right_queue->boat_position = 100;
            break;
    }
}

/**
 * Llena las colas de barcos con la informacion del archivo de configuracion.
 * @param side Lado al que se agrega el barco: cola derecha o cola izquierda.
 * @author Eduardo Bolivar Minguet
 */
void add_boat(const int side) {
    CEthread_t* thread = nullptr;
    char b_type[10];
    strcpy(b_type, get_random_boat());
    if (side == 0) {
        if (strcmp(b_type, "Normal") == 0 && normalBoatsLeft > 0) {
            add_to_queue(&left_queue, id_left++, (double) channelLength / boatSpeed, normalPriority, b_type, 0, thread);
            normalBoatsLeft--;
        } else if (strcmp(b_type, "Pesquero") == 0 && fishingBoatsLeft > 0) {
            add_to_queue(&left_queue, id_left++, (double) channelLength / (1.5 * boatSpeed), fishingPriority, b_type, 0, thread);
            fishingBoatsLeft--;
        } else if (strcmp(b_type, "Patrulla") == 0 && patrolBoatsLeft > 0) {
            // La velocidad asignada a la patrulla depende de su deadline y de la cantidad de patrullas.
            // Esto para lograr que todas las patrullas pasen antes de que se cumpla el deadline
            add_to_queue(&left_queue, id_left++, (double) patrolDeadline / totalPatrolBoats, 0, b_type, 0, thread);
            patrolBoatsLeft--;
        }
    } else {
        if (strcmp(b_type, "Normal") == 0 && normalBoatsRight > 0) {
            add_to_queue(&right_queue, id_right++, (double) channelLength / boatSpeed, normalPriority, b_type, channelLength, thread);
            normalBoatsRight--;
        } else if (strcmp(b_type, "Pesquero") == 0 && fishingBoatsRight > 0) {
            add_to_queue(&right_queue, id_right++, (double) channelLength / (1.5 * boatSpeed), fishingPriority, b_type, channelLength, thread);
            fishingBoatsRight--;
        } else if (strcmp(b_type, "Patrulla") == 0 && patrolBoatsRight > 0) {
            // La velocidad asignada a la patrulla depende de su deadline y de la cantidad de patrullas.
            // Esto para lograr que todas las patrullas pasen antes de que se cumpla el deadline
            add_to_queue(&right_queue, id_right++, (double) patrolDeadline / totalPatrolBoats, 0, b_type, channelLength, thread);
            patrolBoatsRight--;
        }
    }
}

/**
 * Lee el archivo de configuracion para establecer parametros iniciales.
 * @author Eduardo Bolivar Minguet
 */
void read_config() {
    // Open the file
    FILE *file = fopen("../config.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    // Buffer to read each line
    char line[100];

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        char key[50];
        char value[50];

        // Split the line into key and value
        sscanf(line, "%[^=]=%s", key, value);

        // Assign values to variables based on the key
        if (strcmp(key, "Scheduler") == 0) {
            strcpy(scheduler, value);
        } else if (strcmp(key, "Flow") == 0) {
            strcpy(flow, value);
        } else if (strcmp(key, "W") == 0) {
            w = atoi(value);
        } else if (strcmp(key, "TimeSwap") == 0) {
            timeSwap = atof(value);
        } else if (strcmp(key, "ChannelLength") == 0) {
            channelLength = atoi(value);
        } else if (strcmp(key, "BoatSpeed") == 0) {
            boatSpeed = atoi(value);
        } else if (strcmp(key, "NormalBoatsRight") == 0) {
            normalBoatsRight = atoi(value);
        } else if (strcmp(key, "FishingBoatsRight") == 0) {
            fishingBoatsRight = atoi(value);
        } else if (strcmp(key, "PatrolBoatsRight") == 0) {
            patrolBoatsRight = atoi(value);
        } else if (strcmp(key, "NormalBoatsLeft") == 0) {
            normalBoatsLeft = atoi(value);
        } else if (strcmp(key, "FishingBoatsLeft") == 0) {
            fishingBoatsLeft = atoi(value);
        } else if (strcmp(key, "PatrolBoatsLeft") == 0) {
            patrolBoatsLeft = atoi(value);
        } else if (strcmp(key,"NormalPriority") == 0) {
            normalPriority = atoi(value);
        } else if (strcmp(key, "FishingPriority") == 0) {
            fishingPriority = atoi(value);
        } else if (strcmp(key, "PatrolDeadline") == 0) {
            patrolDeadline = atoi(value);
        } else if (strcmp(key, "RRQuantum") == 0) {
            quantum = atof(value);
        }
    }
    totalPatrolBoats = patrolBoatsLeft + patrolBoatsRight;
    tmp1 = patrolBoatsLeft;
    tmp2 = patrolBoatsRight;

    // Close the file
    fclose(file);
}

/**
 * Funcion principal del programa.
 * @return estado de ejecucion
 */
int main() {
    srand(time(nullptr));

    // Lee el archivo de configuracion
    read_config();

    // Verifica que el canal cumpla con limites
    if (channelLength > 500) {
        perror("Channel length too large");
        return 1;
    } if (channelLength < 100) {
        perror("Channel length too small");
        return 1;
    }

    // Agrega barcos a las colas dependiendo de la configuracion
    while (normalBoatsLeft + fishingBoatsLeft + patrolBoatsLeft > 0) {
        // Agrega barco a la izquierda
        add_boat(0);
    }
    while (normalBoatsRight + fishingBoatsRight + patrolBoatsRight > 0) {
        // Agrega barco a la derecha
        add_boat(1);
    }

    patrolBoatsLeft = tmp1;
    patrolBoatsRight = tmp2;

    // Verifica que hayan patrullas en alguna de las colas ya que tienen tratamiento de urgencia
    if (totalPatrolBoats > 0) {
        // Atiende patrullas con algoritmo EDF
        earliest_deadline_first(&left_queue, patrolBoatsLeft, channelLength);
        earliest_deadline_first(&right_queue, patrolBoatsRight, channelLength);
    }

    // Verifica cual control de flujo es el configurado
    if (strcmp(flow, "Equidad") == 0) {

        // Ejecuta equidad
        equity(w, &right_queue, &left_queue, scheduler, quantum, channelLength);

    } else if (strcmp(flow, "Letrero") == 0) {

        // Ejecuta letrero
        signboard(timeSwap, &right_queue, &left_queue, scheduler, quantum, channelLength);

    } else if (strcmp(flow, "Tico") == 0) {

        // Ejecuta tico
        tico(&right_queue, &left_queue, scheduler, quantum,  channelLength);

    } else {
        perror("Unexpected Flow Algorithm");
        return 1;
    }
    return 0;
}
