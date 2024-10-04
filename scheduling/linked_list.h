//
// Created by ejbm0110 on 9/27/24.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <pthread.h>

struct Node {
    int pid, burst_time, priority;
    char boat_type[10];
    pthread_t process;
    struct Node* next;
};

struct Node* create_node(int id, int burst_time, int priority, char* boat_type, pthread_t process);
void add_to_queue(struct Node** queue_ref, int id, int burst_time, int priority, char* boat_type, pthread_t process);
void remove_from_queue(struct Node** queue_ref);
int get_length(struct Node* queue_ref);

#endif //LINKED_LIST_H
