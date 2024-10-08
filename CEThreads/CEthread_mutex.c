//
// Created by njohnson on 10/4/24.
//

#include "CEthread_mutex.h"

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
int create_sem(key_t key, int num_sems)
{
    // Create the semaphore
    int semid = semget(key, num_sems, SEM_FLAGS);

    // Error code is -1
    if(semid < 0)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore to 1
    union semun sem_state;  // This union is necesary to set the
    sem_state.val = 1;      // semaphore state

    if (semctl(semid, 0, SETVAL, sem_state) == -1)
    {
        perror("semctl_initilization");
        exit(EXIT_FAILURE);
    }

    // Return the sem_id
    return semid;
}