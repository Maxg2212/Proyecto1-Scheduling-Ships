//
// Created by njohnson on 10/4/24.
//

#ifndef CETHREAD_TOOLS_H
#define CETHREAD_TOOLS_H

#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


#define FALSE 0
#define TRUE 1


// Structure to pass multiple arguments to the clone function
// and wrap it to call lpthread_end when execution has finished
typedef struct CEthread_arguments
{
    int (*target_function) (void*);
    void* fnctn_args;
} CEthread_arguments_t;




// Functions
/**
 *  This function manages errors on memory allocation
 *
 *  Params:
 *      size_t size - Size in bytes of the allocated memory space.
 *
 *  Returns
 *      void* ptr to memory.
 */
void* allocate_mem(size_t size);


/**
 *  This function uses clone system call to create a thread and manages errors
 *
 *  Params:
 *      size_t size - Size in bytes of the allocated memory space.
 *
 *  Returns
 *      int pid to the new thread.
 */
int clone_call(int (*target_function) (void*),
               void* stack,
               int flags,
               void* args);



#endif //CETHREAD_TOOLS_H
