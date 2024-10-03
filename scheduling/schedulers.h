//
// Created by ejbm0110 on 9/28/24.
//

#ifndef FCFS_H
#define FCFS_H

#include "linked_list.h"

struct Node* round_robin(struct Node* head);
struct Node* priority(struct Node* head);
struct Node* shortest_job_first(struct Node* head);
struct Node* first_come_first_served(struct Node* head);
struct Node* real_time(struct Node* head);

#endif //FCFS_H
