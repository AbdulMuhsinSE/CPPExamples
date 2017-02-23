#ifndef fiq_h
#define fiq_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "data.h"

typedef struct Node{
    Data *data;
    struct Node *next;
    //pthread_mutex_t lock;
}Node;

void fiq_init();
void print();
int getItemsCount();
Node * fiq_getNewNode();
void enqueue(Data *data);
Node * dequeue();
void allocMem(int chunk);

#endif /* fiq_h */
