#include "CEthread.h"

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

// Variable global del contador
int counter = 0;
pthread_mutex_t lock; 


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

void* trythis(void* arg)
{
    pthread_mutex_lock(&lock);

    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d has started\n", counter);

    for (i = 0; i < (0xFFFFFFFF); i++)
        ;

    printf("\n Job %d has finished\n", counter);

    pthread_mutex_unlock(&lock);

    return NULL;
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

int main() {

    CEthread_t* thread;
    CEthread_t* thread2;

    CEthread_create(&thread, NULL, &increment_counter, NULL);
    CEthread_create(&thread2, NULL, &increment_counter, NULL);

    CEthread_join(thread);
    CEthread_join(thread2);


    return 0;
}
