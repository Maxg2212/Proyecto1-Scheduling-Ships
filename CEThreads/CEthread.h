//
// Created by njohnson on 10/4/24.
//

#ifndef CETHREAD_H
#define CETHREAD_H

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <linux/sched.h>

#include <stdlib.h>


#include "CEthread_thread.h"
#include "CEthread_mutex.h"
#include "CEthread_tools.h"


#define MAX_THREADS 100


// Clone flags
static const int clone_flags = CLONE_VM;

// Mantain the system coherent and safe :)
int sannity_check();

// Create thread function
pid_t CEthread_create(CEthread_t** CEthread_ptr,
                     CEthread_attr_t** CEthread_attr,
                     int (*target_function) (void*),
                     void* args);

// Exit the process and set the thread as ready to join
int CEthread_end(void* args);

// Yield back thread
int CEthread_yield();

// Wait for all the joinable threads to terminate
int CEthread_join(CEthread_t* CEthread_ptr);

// Make a thread un-joinable
int CEthread_detach(CEthread_t* CEthread_ptr);

// Initialize a mutex
int CEmutex_init(CEthread_mutex_t** mutex, CEthread_mutex_attr_t** attributes);

// Destroy a mutex
int CEmutex_destroy(CEthread_mutex_t* mutex);

// Unlock a mutex
int CEmutex_unlock(CEthread_mutex_t* mutex);

// Lock a mutex
int CEmutex_trylock(CEthread_mutex_t* mutex);


void add_thread(pid_t target, CEthread_t** CEthread_ptr);
size_t search_target_pid(pid_t pid);


#endif //CETHREAD_H
