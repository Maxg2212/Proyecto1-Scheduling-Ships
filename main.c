#include <stdio.h>
#include <stdlib.h>
#include "channel_control/channel_algorithms.h"
#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"
#include <string.h>

// Configuration parameters
char scheduler[20];
char flow[20];
int w, channelLength, boatSpeed, normalPriority, fishingPriority, patrolDeadline;
int normalBoatsRight, fishingBoatsRight, patrolBoatsRight;
int normalBoatsLeft, fishingBoatsLeft, patrolBoatsLeft;
double timeSwap, quantum;
int totalPatrolBoats;

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
            add_to_queue(&left_queue, id++, 4, 0, get_random_boat(), 0, thread);
            left_queue->boat_position = 0;
            break;
        case 2:
            add_to_queue(&right_queue, id++, 4, 0, get_random_boat(), channelLength, thread);
            right_queue->boat_position = 100;
            break;
    }
}

void add_boat(const int side) {
    constexpr pthread_t thread = 0;
    char b_type[10];
    strcpy(b_type, get_random_boat());
    if (side == 0) {
        if (strcmp(b_type, "Normal") == 0 && normalBoatsLeft > 0) {
            add_to_queue(&left_queue, id++, (double) channelLength / boatSpeed, normalPriority, b_type, 0, thread);
            normalBoatsLeft--;
        } else if (strcmp(b_type, "Pesquero") == 0 && fishingBoatsLeft > 0) {
            add_to_queue(&left_queue, id++, (double) channelLength / (1.5 * boatSpeed), fishingPriority, b_type, 0, thread);
            fishingBoatsLeft--;
        } else if (strcmp(b_type, "Patrulla") == 0 && patrolBoatsLeft > 0) {
            add_to_queue(&left_queue, id++, (double) patrolDeadline / totalPatrolBoats, 0, b_type, 0, thread);
            patrolBoatsLeft--;
        }
    } else {
        if (strcmp(b_type, "Normal") == 0 && normalBoatsRight > 0) {
            add_to_queue(&right_queue, id++, (double) channelLength / boatSpeed, normalPriority, b_type, channelLength, thread);
            normalBoatsRight--;
        } else if (strcmp(b_type, "Pesquero") == 0 && fishingBoatsRight > 0) {
            add_to_queue(&right_queue, id++, (double) channelLength / (1.5 * boatSpeed), fishingPriority, b_type, channelLength, thread);
            fishingBoatsRight--;
        } else if (strcmp(b_type, "Patrulla") == 0 && patrolBoatsRight > 0) {
            add_to_queue(&right_queue, id++, (double) patrolDeadline / totalPatrolBoats, 0, b_type, channelLength, thread);
            patrolBoatsRight--;
        }
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
    // Close the file
    fclose(file);
}


int main() {
    srand(time(nullptr));
    read_config();
    if (channelLength > 500) {
        perror("Channel length too large");
        return 1;
    } if (channelLength < 100) {
        perror("Channel length too small");
        return 1;
    }

    while (normalBoatsLeft + fishingBoatsLeft + patrolBoatsLeft > 0) {
        add_boat(0);
    }
    while (normalBoatsRight + fishingBoatsRight + patrolBoatsRight > 0) {
        add_boat(1);
    }

    //first_come_first_served(&right_queue, channelLength);
    round_robin(&left_queue, 0, quantum, channelLength);
    //earliest_deadline_first(&left_queue, channelLength);
    //if (strcmp(flow, "Equidad") == 0) {
        //equity(w, &right_queue, &left_queue);
    //} else if (strcmp(flow, "Letrero") == 0) {
        //signboard(timeSwap, &right_queue, &left_queue);
    //} else if (strcmp(flow, "Tico") == 0) {
        //tico(&right_queue, &left_queue);
    //} else {
        //perror("Unexpected Flow Algorithm");
        //return 1;
    //}
    return 0;
}
