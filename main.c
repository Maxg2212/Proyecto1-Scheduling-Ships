#include <stdio.h>
#include <pthread.h>

#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"

int main(void) {
    struct Node* head = nullptr;
    int id = 0;
    int hilos = 5;
    while (hilos > 0) {
        pthread_t thread;
        head = add_to_queue(head, id++, thread, hilos, 0);
        hilos--;
    }

    shortest_job_first(head);
    return 0;
}
