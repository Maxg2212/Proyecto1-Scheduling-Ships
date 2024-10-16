//
// Created by ejbm0110 on 9/28/24.
//

#ifndef FCFS_H
#define FCFS_H

#include "linked_list.h"

void priority(struct Node** head, int W, double swapTime);
void shortest_job_first(struct Node** head, int W, double swapTime);
void first_come_first_served(struct Node** head, int W, double swapTime);
void earliest_deadline_first(struct Node** head, int numOfPatrols);
void init_izquierda();
void init_derecha();
void init_mutex();
void kill_mutex();

#endif //FCFS_H
