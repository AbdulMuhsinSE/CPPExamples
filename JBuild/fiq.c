#include "fiq.h"

typedef struct Queue{
    int freeItems;
    struct Node *head;
    struct Node *tail;
    pthread_mutex_t lock;
}Queue;

static Queue *queue = NULL;

void fiq_init(){
    queue = (Queue *)malloc(sizeof(Queue));
    queue->freeItems = 0;
    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_t temp = PTHREAD_MUTEX_INITIALIZER;
    queue->lock = temp;
}

void print(){
    printf("Print\n");
    Node *temp = queue->head;
    if(temp == NULL){
        printf("Empty List");
    }
    while(temp != queue->tail){
        printf("%d\n",temp->data->sID);
        temp = temp->next;
    }
    printf("%d\n",temp->data->sID);
}

int getItemsCount(){
    //printf("Item Count\n");
    int error = pthread_mutex_lock(&queue->lock);
    if(error == 0){
        //printf("Locked\n");
    }
    else{
        //printf("Not Locked, Error: %d\n", error);
    }
    int temp = queue->freeItems;
    error = pthread_mutex_unlock(&queue->lock);
    if(error == 0){
        //printf("Unlocked\n");
    }
    else{
        //printf("Not Unlocked, Error: %d\n", error);
    }
    return temp;
}

Node * fiq_getNewNode(){
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = (Data *)malloc(sizeof(Data));
    node->next = NULL;
    //pthread_mutex_t temp = PTHREAD_MUTEX_INITIALIZER;
    //node->lock = temp;
    return node;
}

void enqueue(Data *data){
    //printf("Enqueue\n");
    //printf("sId: %d, age: %d\n",data->sID, data->age);
    
    int error = pthread_mutex_lock(&queue->lock);
    if(error == 0){
        //printf("Locked\n");
    }
    else{
        //printf("Not Locked, Error: %d\n", error);
    }
    
    if(queue->tail == NULL){
        queue->tail = (Node *)malloc(sizeof(Node));
        queue->tail->next = NULL;
        queue->tail->data = data;
        //pthread_mutex_t temp = PTHREAD_MUTEX_INITIALIZER;
        //queue->tail->lock = temp;
        queue->head = queue->tail;
    }
    else{
        Node *temp = fiq_getNewNode();
        queue->tail->next = temp;
        temp->data = data;
        queue->tail = temp;
    }
    
    queue->freeItems++;
    
    error = pthread_mutex_unlock(&queue->lock);
    if(error == 0){
        //printf("Unlocked\n");
    }
    else{
        //printf("Not Unlocked, Error: %d\n", error);
    }
}

Node * dequeue(){
    //printf("Dequeue\n");
    int error = pthread_mutex_lock(&queue->lock);
    if(error == 0){
        //printf("Locked\n");
    }
    else{
        //printf("Not Locked, Error: %d\n", error);
    }
    
    Node *temp = queue->head;
    if(temp->next != NULL){
        free(queue->head);
        queue->head = temp->next;
    }
    
    queue->freeItems--;
    
    error = pthread_mutex_unlock(&queue->lock);
    if(error == 0){
        //printf("Unlocked\n");
    }
    else{
        //printf("Not Unlocked, Error: %d\n", error);
    }
    
    return temp;
}

void allocMem(int chunk){
    //printf("Allocate\n");
    int error = pthread_mutex_lock(&queue->lock);
    if(error == 0){
        //printf("Locked\n");
    }
    else{
        //printf("Not Locked, Error: %d\n", error);
    }
    //queue->freeItems += chunk;
    error = pthread_mutex_unlock(&queue->lock);
    if(error == 0){
        //printf("Unlocked\n");
    }
    else{
        //printf("Not Unlocked, Error: %d\n", error);
    }
}




