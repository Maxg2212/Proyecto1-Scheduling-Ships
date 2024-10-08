

#include "CEthread.h"


#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


// Variable global del contador
int counter = 0;

CEthread_t tid[2];
int counter;
CEthread_mutex_t *lock;



void* trythis(void* arg)
{
    CEmutex_trylock(lock);

    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d has started\n", counter);

    for (i = 0; i < (0xFFFFFFFF); i++);

    printf("\n Job %d has finished\n", counter);

    CEmutex_unlock(lock);

    return NULL;
}

// Función que incrementa el contador
int increment_counter(void* args) {

    for (int i = 0; i < 5; i++) {
        printf("Counter: %d\n", counter);
        counter++;
    }
    return 0;
}
int hey(void* args){

    sleep(1);

    printf("Hey its me child pid %i, am alive\n", getpid());
    sleep(1);
    printf("Now im gonna die\n");

    return 0;
}

void creation_test(){

    printf("\n\n ------- Now testing creation and killing ------ \n\n");
    CEthread_t* thread;
    int child_pid = CEthread_create(&thread, NULL, &hey, NULL);

    printf( "Hey its me parent, the returned pid was %i and my pid is %i, gonna sleep for a while\n",
            child_pid, getpid());
    sleep(5);

    printf("My time has come my friend, its been a good ride, see ya\n");

}

int no_mutex_count(void* args) {

    sleep(1);

    int pid = getpid();

    printf("Hey its me child pid %i, im gonna print 5 times my pid\n", pid);

    // Lock the mutex
    for (size_t i = 0; i < 5; ++i)
    {
        printf("%i\n", pid);
        sleep(1);
    }
    printf("Done \n\n");

    return 0;
}


int mutex_count(void* args) {
    CEthread_mutex_t* mutex = (CEthread_mutex_t*) args;

    sleep(1);

    int pid = getpid();

    // Lock the mutex
    CEmutex_trylock(mutex);

    printf("Hey its me child pid %i, im gonna print 5 times my pid\n", pid);

    for (size_t i = 0; i < 5; ++i)
    {
        printf("%i\n", pid);
        sleep(1);
    }
    printf("Done \n\n");
    CEmutex_unlock(mutex);

    return 0;
}


void mutex_test(){
    printf("\n\n ------- Now testing non mutex vs mutex printing ------ \n\n");
    CEthread_t* thread1;
    CEthread_t* thread2;
    CEthread_mutex_t* mutex;

    // Create mutex
    CEmutex_init(&mutex, NULL);

    // Launch non mutex threads
    printf("Thread 1 created with pid %i\n", CEthread_create(&thread1, NULL, &no_mutex_count, NULL));
    printf("Thread 2 created with pid %i\n", CEthread_create(&thread2, NULL, &no_mutex_count, NULL));

    sleep(6);
    // Launch mutex threads
    printf("Thread 1 created with pid %i\n", CEthread_create(&thread1, NULL, &mutex_count, mutex));
    printf("Thread 2 created with pid %i\n", CEthread_create(&thread2, NULL, mutex_count, mutex));

    sleep(12);
}

int main() {


    //creation_test();
    mutex_test();

    /**
    CEthread_t* thread;
    CEthread_t* thread2;

    CEthread_create(&thread, NULL, &increment_counter, NULL);
    CEthread_create(&thread2, NULL, &increment_counter, NULL);

    CEthread_join(thread);
    CEthread_join(thread2);
    */


    return 0;
}