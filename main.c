#include <stdio.h>
#include <stdlib.h>
#include "channel_control/channel_algorithms.h"
#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"
#include <string.h>

// Configuration parameters
char scheduler[20];
char flow[20];
int w, channelLength, boatSpeed, boatsInTheRight, boatsInTheLeft;
double timeSwap;

struct Node* left_queue = nullptr;
struct Node* right_queue = nullptr;

int id = 1;

// Function to get a random boat type
const char* get_random_boat() {
    const char* boat_types[3] = {"Normal", "Pesquero", "Patrulla"};
    int random_index = rand() % 3;
    return boat_types[random_index];
}

void add_boat_by_key() {
    pthread_t thread;
    int side = (rand() % 2) + 1;
    switch (side) {
        case 1:
            add_to_queue(&left_queue, id++, 4, 0, get_random_boat(), thread);
            left_queue->boat_position = 0;
            break;
        case 2:
            add_to_queue(&right_queue, id++, 4, 0, get_random_boat(), thread);
            right_queue->boat_position = 100;
            break;
    }
}

void add_boat(int const side) {
    pthread_t thread;
    double boat_burst_time = 0;
    char b_type[10];
    strcpy(b_type, get_random_boat());
    if (strcmp(b_type, "Normal") == 0) {
        boat_burst_time = (double) channelLength / boatSpeed;
    } else if (strcmp(b_type, "Pesquero") == 0) {
        boat_burst_time = (double) channelLength / (1.5 * boatSpeed);
    } else if (strcmp(b_type, "Patrulla") == 0) {
        boat_burst_time = (double) channelLength / (3 * boatSpeed);
    }
    if (side) {
        add_to_queue(&right_queue, id++, boat_burst_time, 0, b_type, thread);
        left_queue->boat_position = 100;
    } else {
        add_to_queue(&left_queue, id++, boat_burst_time, 0, b_type, thread);
        left_queue->boat_position = 0;
    }
}

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
        } else if (strcmp(key, "BoatsInTheRight") == 0) {
            boatsInTheRight = atoi(value);
        } else if (strcmp(key, "BoatsInTheLeft") == 0) {
            boatsInTheLeft = atoi(value);
        }
    }

    // Close the file
    fclose(file);
}


int main() {
    srand(time(NULL));
    read_config();

    if (channelLength > 500) {
        perror("Channel length too large");
        return 1;
    } if (channelLength < 100) {
        perror("Channel length too small");
        return 1;
    }


    for (int i = 0; i < boatsInTheLeft; i++) {
        add_boat(0);
    }
    for (int i = 0; i < boatsInTheRight; i++) {
        add_boat(1);
    }
    if (strcmp(flow, "Equidad") == 0) {
        equity(w, &right_queue, &left_queue);
    } else if (strcmp(flow, "Letrero") == 0) {
        signboard(timeSwap, &right_queue, &left_queue);
    } else if (strcmp(flow, "Tico") == 0) {
        tico(&right_queue, &left_queue);
    } else {
        perror("Unexpected Flow Algorithm");
        exit(1);
    }
    return 0;
}
