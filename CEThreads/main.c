#include <stdio.h>
#include "CEThreads.h"

// Counter variable
int counter = 0;

// Function that increments the counter
void increment_counter(void *arg) {
    int id = *((int *)arg); // Get thread ID from argument
    for (int i = 0; i < 5; i++) {
        printf("Thread %d - Counter: %d\n", id, counter);
        counter++;
    }
}

int main() {
    CEThread threads[2]; // Create two threads
    int thread_ids[2] = {0, 1};

    // Create threads
    create(&threads[0], increment_counter, &thread_ids[0]);
    create(&threads[1], increment_counter, &thread_ids[1]);

    // Manually invoke the thread functions in the main function
    for (int i = 0; i < 2; i++) {
        // Run each thread's function
        threads[i].function(threads[i].arg);
    }

    return 0;
}
