//
// Created by ejbm0110 on 9/30/24.
//

#ifndef CHANNEL_ALGORITHMS_H
#define CHANNEL_ALGORITHMS_H

#include "../scheduling/linked_list.h"

void equity(int W, struct Node** r_queue, struct Node** l_queue, char scheduler[20], double rr_quantum);
void signboard(double swap_time, struct Node** r_queue, struct Node** l_queue, char scheduler[20], double rr_quantum);
void tico(struct Node** r_queue, struct Node** l_queue, char scheduler[20], double rr_quantum);

#endif //CHANNEL_ALGORITHMS_H
