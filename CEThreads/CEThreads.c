//
// Created by michael-v
//

#include "CEThreads.h"
#include <stdlib.h>
#include <stdio.h>

CEThread *current_thread = NULL;

//implementación de create
int create(CEThread *t, void (*function)(void *), void *arg) {
    printf("Creating thread...\n");
    t->function = function;
    t->arg = arg;
    t->status = 1; // se marca con 1 como activo
    t->stack = malloc(STACK_SIZE); // se asigna memoria para la pila del hilo

    if(!t->stack) return -1;  // se no se puede asignar se retorna -1

    //para guardar contexto inicial del hilo
    if(setjmp(t->env) == 0) {
        return 0;
    } else {
        //cuando se ejecuta el hilo, se llama a la función
        t->function(t->arg);
        t->status = 0; // se marca como terminado o inactivo cuando la función finaliza
        free(t->stack); //se libera memoria
        return 0;
    }
}

// implementación de end
void end(CEThread *t) {
    t->status = 0;          // marcar el hilo como terminado
    free(t->stack);         // liberar la memoria de la pila
}

//implementación de join
void join(CEThread *t) {
    printf("Waiting for thread to finish...\n");
    // Esperar hasta que el hilo termine (status = 0)
    while (t->status == 1) {

    }
}



