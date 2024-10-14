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
double timeSwap;

int totalPatrolBoats, tmp1, tmp2;

struct Node* left_queue = nullptr;
struct Node* right_queue = nullptr;

int id_left = 1;
int id_right = 1;

int running_global = 0;

// --------------------------------------------------------------------------------
//Comentario equis
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

void add_boat_by_key(int queue) {
    char boat[10];
    strcpy(boat, get_random_boat());
    CEthread_t* thread = nullptr;
    int x_init;
    if (queue == 1) {
        x_init = 900 - channelLength / 2 - (get_length(left_queue) + 1) * 100;
        if (strcmp(boat, "Normal") == 0) {
            add_to_queue(&left_queue, id_left++, normalPriority, boat, boatSpeed, x_init, 230, thread, channelLength);
        } else if (strcmp(boat, "Pesquero") == 0) {
            add_to_queue(&left_queue, id_left++, fishingPriority, boat, 2 * boatSpeed, x_init, 230, thread, channelLength);
        } else if (strcmp(boat, "Patrulla") == 0) {
            // La velocidad asignada a la patrulla depende de su deadline y de la cantidad de patrullas.
            // Esto para lograr que todas las patrullas pasen antes de que se cumpla el deadline
            add_to_queue(&left_queue, id_left++, 0, boat, patrolDeadline, x_init, 230, thread, channelLength);
            patrolBoatsLeft++;
        }
    } else if (queue == 2) {
        x_init = 900 + channelLength / 2 + 20 + get_length(right_queue) * 100;

        if (strcmp(boat, "Normal") == 0) {
            add_to_queue(&right_queue, id_right++, normalPriority, boat, boatSpeed, x_init, 370, thread, channelLength);
        } else if (strcmp(boat, "Pesquero") == 0) {
            add_to_queue(&right_queue, id_right++, fishingPriority, boat, 2 * boatSpeed, x_init, 370, thread, channelLength);
        } else if (strcmp(boat, "Patrulla") == 0) {
            // La velocidad asignada a la patrulla depende de su deadline y de la cantidad de patrullas.
            // Esto para lograr que todas las patrullas pasen antes de que se cumpla el deadline
            add_to_queue(&right_queue, id_right++, 0, boat, patrolDeadline, x_init, 370, thread, channelLength);
            patrolBoatsRight++;
        }
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

    int x_init = 0;

    if (side == 0) {

        x_init = 900 - channelLength / 2 - (get_length(left_queue) + 1) * 100;

        if (strcmp(b_type, "Normal") == 0 && normalBoatsLeft > 0) {
            add_to_queue(&left_queue, id_left++, normalPriority, b_type, boatSpeed, x_init, 230, thread, channelLength);
            normalBoatsLeft--;
        } else if (strcmp(b_type, "Pesquero") == 0 && fishingBoatsLeft > 0) {
            add_to_queue(&left_queue, id_left++, fishingPriority, b_type, 2 * boatSpeed, x_init, 230, thread, channelLength);
            fishingBoatsLeft--;
        } else if (strcmp(b_type, "Patrulla") == 0 && patrolBoatsLeft > 0) {
            // La velocidad asignada a la patrulla depende de su deadline y de la cantidad de patrullas.
            // Esto para lograr que todas las patrullas pasen antes de que se cumpla el deadline
            add_to_queue(&left_queue, id_left++, 0, b_type, patrolDeadline, x_init, 230, thread, channelLength);
            patrolBoatsLeft--;
        }
    } else {

        x_init = 900 + channelLength / 2 + 20 + get_length(right_queue) * 100;

        if (strcmp(b_type, "Normal") == 0 && normalBoatsRight > 0) {
            add_to_queue(&right_queue, id_right++, normalPriority, b_type, boatSpeed, x_init, 370, thread, channelLength);
            normalBoatsRight--;
        } else if (strcmp(b_type, "Pesquero") == 0 && fishingBoatsRight > 0) {
            add_to_queue(&right_queue, id_right++, fishingPriority, b_type, 2 * boatSpeed, x_init, 370, thread, channelLength);
            fishingBoatsRight--;
        } else if (strcmp(b_type, "Patrulla") == 0 && patrolBoatsRight > 0) {
            // La velocidad asignada a la patrulla depende de su deadline y de la cantidad de patrullas.
            // Esto para lograr que todas las patrullas pasen antes de que se cumpla el deadline
            add_to_queue(&right_queue, id_right++, 0, b_type, patrolDeadline, x_init, 370, thread, channelLength);
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
        }
    }
    totalPatrolBoats = patrolBoatsLeft + patrolBoatsRight;
    tmp1 = patrolBoatsLeft;
    tmp2 = patrolBoatsRight;

    // Close the file
    fclose(file);
}

void* gui_thread(void* arg) {
    init_gui();
    int running = 1;
    uint32_t event_data[2];

    while (running) {
        get_event(event_data);

        if (event_data[0] == SDL_QUIT) {
            running = 0;
            running_global = 0;

            // Thread end de todos los barcos

        }

        if (event_data[0] == SDL_KEYDOWN) {
            if (event_data[1] == SDLK_a) {
                add_boat_by_key(1);
            } else if (event_data[1] == SDLK_d) {
                add_boat_by_key(2);
            }
        }

        render_gui(&left_queue, &right_queue, channelLength);
        SDL_Delay(16);
    }
    destroy_gui();
    return nullptr;
}

/**
 * Funcion principal del programa.
 * @return estado de ejecucion
 */
int main() {
    srand(time(nullptr));

    CEthread_t* main_gui = nullptr;

    // Lee el archivo de configuracion
    read_config();

    // Limitar el largo de canal a valores minimos y maximos
    if (channelLength > 1560) {
        channelLength = 1560;
    } else if (channelLength < 500) {
        channelLength = 500;
    }

    // Limitar la velocidad de los barcos
    if (boatSpeed > 5) {
        boatSpeed = 5;
    } else if (boatSpeed < 1) {
        boatSpeed = 1;
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

    running_global = 1;

    CEthread_create(&main_gui, nullptr, gui_thread, nullptr);

    patrolBoatsLeft = tmp1;
    patrolBoatsRight = tmp2;

    init_mutex();   // Inicializar mutex

    while (running_global) {

        totalPatrolBoats = patrolBoatsLeft + patrolBoatsRight;

        if (totalPatrolBoats > 0) {
            earliest_deadline_first(&left_queue, patrolBoatsLeft);
            earliest_deadline_first(&right_queue, patrolBoatsRight);

            patrolBoatsLeft = 0;
            patrolBoatsRight = 0;
        }

        if (strcmp(flow, "Equidad") == 0) {
            equity(w, &right_queue, &left_queue, scheduler);
        } else if (strcmp(flow, "Letrero") == 0) {
            signboard(timeSwap, &right_queue, &left_queue, scheduler);
        } else if (strcmp(flow, "Tico") == 0) {
            tico(&right_queue, &left_queue, scheduler);
        }
    }

    CEthread_join(main_gui);

    return 0;
}
