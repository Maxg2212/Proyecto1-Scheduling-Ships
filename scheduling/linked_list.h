//
// Created by ejbm0110 on 9/27/24.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "../CEThreads/CEthread.h"

struct Node {
    int pid, priority;
    char boat_type[10];
    double burst_time, boat_position;
    CEthread_t* process;
    struct Node* next;
};

struct Node* create_node(int id, double burst_time, int priority, char boat_type[10], double position, CEthread_t* process);
void add_to_queue(struct Node** queue_ref, int id, double burst_time, int priority, char* boat_type, double position, CEthread_t* process);
void remove_from_queue(struct Node** queue_ref);
int get_length(struct Node* queue_ref);

#endif //LINKED_LIST_H
