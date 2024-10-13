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
    int speed;
    int x, y;
    CEthread_t* t;
    int channel;
    struct Node* next;
};

struct Node* create_node(int pid, int priority, char* boat_type, int speed, int x, int y, CEthread_t* t, int channel);
void add_to_queue(struct Node** queue_ref, int pid, int priority, char* boat_type, int speed, int x, int y, CEthread_t* t, int channel);
void remove_from_queue(struct Node** queue_ref);
int get_length(const struct Node* queue_ref);

#endif //LINKED_LIST_H
