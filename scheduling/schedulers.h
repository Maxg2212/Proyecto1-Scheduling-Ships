//
// Created by ejbm0110 on 9/28/24.
//

#ifndef FCFS_H
#define FCFS_H

#include "linked_list.h"

void round_robin(struct Node** head, int offset, double quantum, int length);
void priority(struct Node** head, int length);
void shortest_job_first(struct Node** head, int length);
void first_come_first_served(struct Node** head, int length);
void earliest_deadline_first(struct Node** head, int length);

#endif //FCFS_H
