//
// Created by ejbm0110 on 9/27/24.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <pthread.h>

struct Node {
    int pid;
    pthread_t process;
    struct Node* next;
};

struct Node* create_node(int id, pthread_t process);
struct Node* add_to_queue(struct Node* head, int id, pthread_t process);
struct Node* remove_from_queue(struct Node* head);

#endif //LINKED_LIST_H
