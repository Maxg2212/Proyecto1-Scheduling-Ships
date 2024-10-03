#include <stdio.h>
#include <pthread.h>
#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"

int id = 0;
struct Node* cola = nullptr;

void add_threads(int const num_de_hilos) {
    for (int i = 0; i < num_de_hilos; i++) {
        pthread_t thread;
        cola = add_to_queue(cola, id++, thread, 10, 10);
    }
}

int main(void) {
    add_threads(4);
    printf("RR\n");
    cola = round_robin(cola);

    add_threads(1);
    printf("FCFS\n");
    first_come_first_served(cola);
}
