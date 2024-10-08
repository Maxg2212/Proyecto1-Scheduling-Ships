//
// Created by njohnson on 10/4/24.
//

#ifndef CETHREAD_MUTEX_H
#define CETHREAD_MUTEX_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>


// Define mutex attributes
#define MUTEX_NAME_LEN 10


// Structure to operate the semaphore
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

static key_t sem_counter = 0;
// Post and wait
static struct sembuf sem_wait = {0, -1, 0};
static struct sembuf sem_post = {0, +1, 0};


// Define posible mutex characteristics
enum CEthread_mutex_type
{
    NORMAL,
    ERRORCHECK,
    RECURSIVE,
    DEFAULT,
};


typedef struct CEthread_mutex_attr
{
    enum CEthread_mutex_type    type;
} CEthread_mutex_attr_t;

// Define a mutex as a semaphore
// Semaphore flags
static const int SEM_FLAGS = 0666 | IPC_CREAT;


typedef struct CEthread_mutex
{
    CEthread_mutex_attr_t*  attributes;
    int                     sem_id;
} CEthread_mutex_t;


// Functions
/**
 *  This function creates a semaphore and manages errors
 *
 *  Params:
 *      key_t key       -       Id to the semaphore.
 *      int num_sems    -       Number of semphores to be created.
 *      int flags       -       Creation flags of the semaphore.
 *
 *  Returns
 *      int id of the created semaphore.
 */
int create_sem(key_t key, int num_sems);

#endif //CETHREAD_MUTEX_H
