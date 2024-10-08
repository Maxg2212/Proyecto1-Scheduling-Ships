//
// Created by njohnson on 10/4/24.
//

#include "CEthread.h"

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


// Define manager thread
CEthread_t manager_thread;
/* Array of threads data

    MAX_THREADS:            Defines the maximum available threads

    short isInitialized:    Defines if the array is initialized

    pid_t threads:          Array with the unkilled threads

 */
short isInitialized = 0;
pid_t threads_pid[MAX_THREADS];
CEthread_t* threads_ptr[MAX_THREADS];


// Functions

/**
 *  This function makes sure that the process list is instantiated and
 *  checks if therea are processes that finished and need to be removed from
 *  the threads list.
 *
 *  Returns
 *     int 0 if there are free spaces in the threads array, -1 if it's full
 */
int sannity_check(){

    // Initialize the thread array in case this is the first time called
    if(!isInitialized){

        // Initialize sandom seed generator
        srand(time(NULL));

        // Set all the pid's to -1 wich means freshly unused
        for (size_t i = 0; i < MAX_THREADS; ++i)
        {
            threads_pid[i] = -1;
        }
        // Set the array as initialized
        isInitialized = 1;
        // There are free spaces so return 0
        return 0;
    }

}




/**
 * This function creates a thread
 *
 * Params:
 *      CEthread_t** CEthread_ptr       -   Variable to return a reference of the created
 *                                          CEthread structure to caller
 *
 *      CEthread_attr_t** CEthread_attr -   Used to return the atributes of the created
 *                                          CEthread to the caller
 *
 *      void* (*target_function) (void*)-   Function to execute
 *
 *      void* args                      -   Arguments of the function
 *
 * Returns
 *     pid_t pid of the created process.
 */
pid_t CEthread_create(CEthread_t** CEthread_ptr,
                    CEthread_attr_t** CEthread_attr,
                    int (*target_function) (void*),
                    void* args)
{

    // Sannity check allways before doing anything
    sannity_check();

    // Allocate stack for child task.
    char* stack = allocate_mem(STACK_SIZE);

    // Launch the thread
    // Set it to stop and start it later when the structure is ready
    CEthread_arguments_t* clone_args = allocate_mem(sizeof(CEthread_arguments_t));
    clone_args->target_function = target_function;
    clone_args->fnctn_args = args;

    pid_t thread_id = clone_call(CEthread_end, stack + STACK_SIZE, clone_flags, clone_args);

    // Allocate memory for the attributes and the structure
    CEthread_t* ptr = allocate_mem(sizeof(CEthread_t));
    CEthread_attr_t* attr = allocate_mem(sizeof(CEthread_attr_t));

    // Instantiate the CEthread_attr structure
    attr->flags = clone_flags;
    attr->stackaddr_attr = stack;
    attr->stacksize_attr = STACK_SIZE;

    // Instantiate the CEthread structure
    ptr->thread_id = thread_id;
    ptr->state = RUNNING;
    ptr->attributes = attr;

    // If we have a structure to reference then
    // update the ptr
    if(CEthread_ptr != NULL){
        *CEthread_ptr = ptr;
    }
    // Same for attr
    if(CEthread_attr != NULL){
        *CEthread_attr = attr;
    }

    // Add it to thre array of threads
    add_thread(thread_id, &ptr);

    return thread_id;

}



/**
 * This function kills a thread
 *
 * Params:
 *      struct CEthreads_arguments* clone_args  -   container for the function and args
 *
 */
int CEthread_end(void* args)
{
    CEthread_arguments_t* clone_args = (CEthread_arguments_t*) args;
    // Execute the function with the given arguments
    clone_args->target_function(clone_args->fnctn_args);
    // Free the memory
    free(clone_args);

    // Get the pid
    int pid = getpid();

    // Then exit safetly
    // Search for the target pid
    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        // If found
        if(threads_pid[i] == pid){
            // Set it to -2 wich means free mem when reusing
            threads_pid[i] = -2;
            // Change the state
            threads_ptr[i]->state = FINISHED;
        }
    }

    return 0;
}





int CEthread_yield(){

    if (sched_yield() == -1)
    {
        perror("CEthread_yield");
        exit(EXIT_FAILURE);
    }
    return 0;
}


int CEthread_join(CEthread_t* thread_ptr){

    // If the thread is not joinable, then error
    if(thread_ptr->state == DETACHED){
        perror("CEthread_join");
        exit(EXIT_FAILURE);
    }

    // Else wait for the thread to change state to
    // FINISHED
    while (thread_ptr->state != FINISHED)
    {
        // If it hasnt, then yield
        CEthread_yield();
    }


}


int CEthread_detach(CEthread_t* thread_ptr){

    // Search for the target
    size_t pos = search_target_pid(thread_ptr->thread_id);

    threads_ptr[pos]->state = DETACHED;

    return 0;

}





/**
 *  This function creates a mutex
 *
 *  Params:
 *      CEthread_mutex_t** mutex            -   Reference to the ptr of the created mutex
 *      CEthread_mutex_attr_t** attributes  -   Reference to the ptr of the created mutex attributes

 *
 *  Returns
 *      int succes on the operation
 */
int CEmutex_init(CEthread_mutex_t** mutex, CEthread_mutex_attr_t** attributes){

    // As always sanity check
    sannity_check();

    // Allocate memory for the attributes and the MUTEX
    CEthread_mutex_t* ptr = allocate_mem(sizeof(CEthread_mutex_t));
    CEthread_mutex_attr_t* attr = allocate_mem(sizeof(CEthread_mutex_attr_t));


    // Set mutex type
    attr->type = DEFAULT;

    // Create the mutex structure
    ptr->attributes = attr;
    ptr->sem_id = create_sem(sem_counter++, 1);

    // If we have a structure to reference then
    // update the ptr
    if(mutex != NULL){
        *mutex = ptr;
    }
    // Same for attr
    if(attributes != NULL){
        *attributes = attr;
    }


    return 0;
}

/**
 *  This function destroys a mutex
 *
 *  Params:
 *      CEthread_mutex_t* mutex     -       Reference to the mutex to be deleted
 *
 *  Returns
 *      int success of the operation
 */
int CEmutex_destroy(CEthread_mutex_t* mutex){

    // As always sanity check
    sannity_check();

    // Close the semaphore
    union semun sem_state;
    if (semctl(mutex->sem_id, 0, IPC_RMID, sem_state) == -1)
    {
        perror("CEmutex_destroy");
        exit(EXIT_FAILURE);
    }

    // Free the memory
    free(mutex->attributes);
    free(mutex);

    return 0;
}

/**
 *  This function creates a thread
 *
 *  Params:
 *      CEthread_mutex_t* mutex     -       Reference to the mutex to be unlocked
 *
 *  Returns
 *      int success of the operation
 */
int CEmutex_unlock(CEthread_mutex_t* mutex){

    // Value of the semaphore
    if (semop(mutex->sem_id, &sem_post, 1) == -1) {
        perror("CEmutex_unlock");
        exit(EXIT_FAILURE);

    }
    return 0;

}

/**
 *  This function creates a thread
 *
 *  Params:
 *      CEthread_mutex_t* mutex     -       Reference to the mutex to be locked
 *
 *  Returns
 *      int success of the operation
 */
int CEmutex_trylock(CEthread_mutex_t* mutex){

    // Value of the semaphore
    if (semop(mutex->sem_id, &sem_wait, 1) == -1) {
        perror("CEmutex_trylock");
        exit(EXIT_FAILURE);

    }

}




/*
 *  This function searches for a free space in the thread array and
 *  sets its pid value
 *
 *
 *  Params:
 *      pid_t target - Identifier of the new process.
 */
void add_thread(pid_t target, CEthread_t** CEthread_ptr){

    // Manage error in case we are full
    int isFound = FALSE;

    // Check all spaces
    for (size_t i = 0; i < MAX_THREADS; i++)
    {
        // If the space is unused then fill it
        if(threads_pid[i] == -1){
            // Update found
            isFound = TRUE;
            // Set new pid
            threads_pid[i] = target;
            // Set ptr for management
            threads_ptr[i] = *CEthread_ptr;
            break;
        }

        // If the space is unused but filled with thrash
        else if(threads_pid[i] == -2){
            // Update found
            isFound = TRUE;
            // Set new pid
            threads_pid[i] = target;
            // Free current thrash
            free(threads_ptr[i]);
            // Set ptr for management
            threads_ptr[i] = *CEthread_ptr;

        }
    }

    if(!isFound){
        perror("add_thread");
        exit(EXIT_FAILURE);
    }

}


/*
 *  This function searches for the pos of a target pid in the pid array
 *
 *  Params:
 *      pid_t target - Identifier of the target process.
 *
 *  Returns
 *      size_t position of the target in the array
 */
size_t search_target_pid(pid_t pid){

    // Index of the target
    size_t i;
    short isFound = FALSE;

    for (i = 0; i < MAX_THREADS; i++)
    {
        // If the target is found
        if(threads_pid[i] == pid){
            // Set as found
            isFound = TRUE;
            // stop the cycle
            break;
        }
    }

    // Manage in case it wasnt found
    if(!isFound){
        perror("search_target_pid");
        exit(EXIT_FAILURE);
    }

    return i;

}