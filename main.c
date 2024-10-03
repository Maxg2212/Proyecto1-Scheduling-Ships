#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"
#include <pthread.h>

int main(void)
{
    int id = 0;
    struct Node* cola = nullptr;
    int hilos = 3;
    while (hilos > 0) {
        pthread_t thread;
        cola = add_to_queue(cola, id++, thread, 10, 0);
        hilos--;
    }
    round_robin(cola);
    return 0;
}
