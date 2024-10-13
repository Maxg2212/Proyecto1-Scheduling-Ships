//
// Created by ejbm0110 on 9/28/24.
//

#ifndef FCFS_H
#define FCFS_H

#include "linked_list.h"

void round_robin(struct Node** head, int W, double swapTime, double local_quantum);
void priority(struct Node** head, int W, double swapTime);
void shortest_job_first(struct Node** head, int W, double swapTime);
void first_come_first_served(struct Node** head, int W, double swapTime);
void earliest_deadline_first(struct Node** head, int numOfPatrols);
void init_mutex();

#endif //FCFS_H
