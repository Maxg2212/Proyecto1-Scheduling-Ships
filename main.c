#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "channel_control/channel_algorithms.h"
#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"

struct Node* left_queue = nullptr;
struct Node* right_queue = nullptr;

int id = 0;

void add_threads(struct Node** queue, int const num_de_hilos) {
    for (int i = 0; i < num_de_hilos; i++) {
        pthread_t thread;
        add_to_queue(queue, id++, 10, 10, "normal", thread);
    }
}

int main(void) {
    add_threads(&left_queue, 5);

    remove_from_queue(&left_queue);

    struct Node* current_queue = left_queue;
    while (current_queue != nullptr) {
        printf("Hilo %d\n", current_queue->pid);
        current_queue = current_queue->next;
    }
}
