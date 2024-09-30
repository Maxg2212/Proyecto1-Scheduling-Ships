#include <stdio.h>
#include "scheduling/linked_list.h"
#include "scheduling/schedulers.h"
#include "channel_control/channel_algorithms.h"

int id = 0;

int main(void) {
    struct Node* filaDerecha = nullptr;
    struct Node* filaIzquierda = nullptr;

    int barcosDerecha = 7;
    int barcosIzquierda = 4;

    while (barcosDerecha > 0) {
        pthread_t thread;
        filaDerecha = add_to_queue(filaDerecha, id++, thread);
        barcosDerecha--;
    }
    while (barcosIzquierda > 0) {
        pthread_t thread;
        filaIzquierda = add_to_queue(filaIzquierda, id++, thread);
        barcosIzquierda--;
    }

    //equity(4, filaDerecha, filaIzquierda);

    //signboard(0.0001, filaDerecha, filaIzquierda);

    //tico(filaDerecha, filaIzquierda);
}
