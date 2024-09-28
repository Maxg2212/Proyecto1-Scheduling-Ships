#include "scheduling/linked_list.h"
#include "scheduling/fcfs.h"

struct Node* head = nullptr;

int main(void) {
    pthread_t t1, t2, t3, t4;
    head = add_to_queue(head, 3, t1);
    head = add_to_queue(head, 4, t2);
    head = add_to_queue(head, 2, t3);
    head = add_to_queue(head, 1, t4);

    first_come_first_served(head);

    return 0;
}
