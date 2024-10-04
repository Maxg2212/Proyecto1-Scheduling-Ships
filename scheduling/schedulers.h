//
// Created by ejbm0110 on 9/28/24.
//

#ifndef FCFS_H
#define FCFS_H

#include "linked_list.h"

void round_robin(struct Node** head);
void priority(struct Node** head);
void shortest_job_first(struct Node** head);
void first_come_first_served(struct Node** head);
void real_time(struct Node** head);

#endif //FCFS_H
