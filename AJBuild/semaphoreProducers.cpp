#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CONSUMERS 1
#define PRODUCERS 2
#define CONSUMER_SLEEP_TIME 25000
#define PRODUCER_SLEEP_TIME 20000
#define MAIN_SLEEP_TIME 500000
#define BUFFER_SIZE 4

int theArray[BUFFER_SIZE], size = 0;
sem_t semConsumer;
sem_t semProducer;

int readCount;
bool working;
int check;

void *produce(void *arg)
{
    int *id = (int *)arg;
  int i = 0;
  while(1)
  {
    if(i==5){
      break;
    }

    if(size == BUFFER_SIZE)
    {
      printf("Producer %d waiting...\n", id);
      sem_wait(&semProducer);
    }

    sem_wait(&semProducer);
    theArray[size] = i;
    printf("Producer %d added %d.\n", id, size);
    size++;
    usleep(1000);
    sem_post(&semProducer);

    sem_getvalue(&semConsumer,&check);
    if (size == 1 && check != 1 )
    {
      sem_post(&semConsumer);
    }


    usleep(PRODUCER_SLEEP_TIME);
    i++;
  }
  return NULL;
}

void *consume(void *arg)
{
  int item;
  int i = 0;
  while(1)
  {
    if(i==10){
      break;
    }

    if (size == 0)
    {
      printf("Consumer waiting....\n");
      sem_wait(&semConsumer);
    }

    sem_wait(&semConsumer);
    item = theArray[--size];
    printf("Consumer removed %d.\n", size);
    //size--;
    usleep(1000);
    sem_post(&semConsumer);


    if (size == BUFFER_SIZE-1)
    {
      sem_post(&semProducer);
    }



    usleep(CONSUMER_SLEEP_TIME);
    i++;
  }
  working = false;
  return NULL;
}

int main(int argc, char* argv[])
{
  pthread_t conThreads[CONSUMERS], prodThreads[PRODUCERS];
  int i,rc,c[CONSUMERS],p[PRODUCERS];

  sem_init(&semProducer,0,1);
  sem_init(&semConsumer,0,1);
  working = true;
  for(i=0; i<PRODUCERS; i++) {
      p[i]=i;
      rc = pthread_create(&prodThreads[i], NULL, produce, (void *)&p[i]);

      if(rc){
          perror("In producer create method");
          exit(1);
      }
      usleep(20000);
  }

  for(i=0; i<CONSUMERS; i++) {
      c[i]=i;
      rc = pthread_create(&conThreads[i], NULL, consume, (void *)&c[i]);

      if(rc){
          perror("In producer create method");
          exit(1);
      }
      usleep(20000);
  }

  while(working){
    usleep(100000);
  }

  for(i = 0; i < CONSUMERS; ++i)
      pthread_cancel(conThreads[i]);

  for(i = 0; i < PRODUCERS; ++i)
      pthread_cancel(prodThreads[i]);


  sem_destroy(&semConsumer);
  sem_destroy(&semProducer);


  return 0;
}
