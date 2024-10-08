//
// Created by ejbm0110 on 9/28/24.
//

#ifndef FCFS_H
#define FCFS_H

#include "linked_list.h"

void round_robin(struct Node** head, int numOfThreads, double local_quantum, int length);
void priority(struct Node** head, int numOfThreads, int length);
void shortest_job_first(struct Node** head, int numOfThreads, int length);
void first_come_first_served(struct Node** head, int numOfThreads, int length);
void earliest_deadline_first(struct Node** head, int numOfPatrols, int length);

#endif //FCFS_H
