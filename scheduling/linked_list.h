//
// Created by ejbm0110 on 9/27/24.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <pthread.h>

struct Node {
    int pid;
    pthread_t process;
    double burst_time;
    int priority;
    struct Node* next;
};

struct Node* create_node(int id, pthread_t process, double burst_time, int priority);
struct Node* add_to_queue(struct Node* head, int id, pthread_t process, double burst_time, int priority);
struct Node* remove_from_queue(struct Node* head);
int get_length(struct Node* head);

#endif //LINKED_LIST_H
