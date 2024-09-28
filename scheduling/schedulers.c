//
// Created by ejbm0110 on 9/28/24.
//

#include "schedulers.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* thread_func(void* arg) {
    printf("Hello World from Thread %d\n", *(int*)arg);
    sleep(2);
}

void first_come_first_served(struct Node* head) {
    while (head != nullptr) {
        pthread_create(&head->process, NULL, thread_func, &head->pid);
        pthread_join(head->process, NULL);
        head = remove_from_queue(head);
    }
}