//
// Created by ejbm0110 on 9/27/24.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "../CEThreads/CEthread.h"

struct Node {
    int pid;
    int priority;
    char boat_type[10];
    double burst_time;
    int x, y;
    CEthread_t* t;
    struct Node* next;
};

struct Node* create_node(int pid, int priority, char* boat_type, double burst_time, int x, int y, CEthread_t* t);
void add_to_queue(struct Node** queue_ref, int pid, int priority, char* boat_type, double burst_time, int x, int y, CEthread_t* t);
void remove_from_queue(struct Node** queue_ref);
int get_length(const struct Node* queue_ref);

#endif //LINKED_LIST_H
