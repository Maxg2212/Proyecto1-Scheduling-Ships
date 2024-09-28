//
// Created by ejbm0110 on 9/27/24.
//

#include<stdio.h>
#include<stdlib.h>
#include"linked_list.h"

struct Node* create_node(int id, pthread_t const process) {
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->pid = id;
    new_node->process = process;
    new_node->next = nullptr;
    return new_node;
}
struct Node* add_to_queue(struct Node* head, int const id, pthread_t const process) {
    if (head == nullptr) {
        return create_node(id, process);
    }
    struct Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = create_node(id, process);
    return head;
}

struct Node* remove_from_queue(struct Node* head) {
    struct Node* temp = head;
    head = head->next;
    free(temp);
    return head;
}
