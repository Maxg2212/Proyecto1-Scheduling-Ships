//
// Created by michael-v
//

#ifndef CETHREADS_H
#define CETHREADS_H

#include <setjmp.h>


#define STACK_SIZE 1024



typedef struct CEThread{
    int thread_id;              // Identificador del hilo
    void (*function)(void *);   // Función que ejecutará el hilo
    void *arg;                  // Argumentos de la función
    int status;                 // 1 para activo, 0 para terminado
    void *stack;                // Pila del hilo
    jmp_buf env;                // Contexto del hilo
} CEThread;


int create(CEThread *t, void (*function)(void *), void *arg);
void end(CEThread *t);
void join(CEThread *t);

extern CEThread *current_thread;

#endif //CETHREADS_H
