#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define CONSUMERS 2
#define PRODUCERS 1
#define CONSUMER_SLEEP_TIME 60000
#define PRODUCER_SLEEP_TIME 20000
#define MAIN_SLEEP_TIME 5000000
#define BUFFER_SIZE 4

int theArray[BUFFER_SIZE], size=0;
sem_t semConsumer;
sem_t semProducer;

pthread_mutex_t myMutex;
pthread_cond_t myCV;  /* declare condition variable object */
int i = 0;

void *produce(void *arg) {
        int *id = (int *)arg;

        //for (i = 0; i<5; i++) {
        while(1) {
                //sem_wait(&semProducer);

                //pthread_mutex_lock(&myMutex); /* lock mutex */

                if (size == BUFFER_SIZE) {
                        printf("Producer %d waiting...\n", id);

                        /* wait for a space to be freed up */
                        //sem_post(&semConsumer);
                        sem_wait(&semProducer);
                        //pthread_cond_wait(&myCV, &myMutex);
                }

                //i++;
                theArray[size] = ++i;
                printf("Producer %d added %d.\n", id, i);
                size++;

                if (size >= 1) {
                        /* signal consumer to resume */
                        sem_post(&semConsumer);
                }

                //pthread_mutex_unlock(&myMutex); /* unlock mutex */

                /* sleep for 1 millisecond, so that other threads can run */
                usleep(PRODUCER_SLEEP_TIME);
        }
        return NULL;
}

void *consume(void *arg) {
        int item;

        //for (i=0; i<10; i++) {
        while (1) {
                //sem_wait(&semConsumer);
                //pthread_mutex_lock(&myMutex); /* lock mutex */

                if (size == 0) {
                        printf("Consumer waiting...\n");

                        /* waiting for an item to be added */
                        //sem_post(&semProducer);
                        sem_wait(&semConsumer);
                        //pthread_cond_wait(&myCV, &myMutex);
                }

                item = theArray[size-1];
                printf("Consumer removed %d.\n", item);
                size--;

                if (size <= BUFFER_SIZE-1) {
                        /* signal producer to resume */
                        sem_post(&semProducer);
                        //pthread_cond_signal(&myCV);
                }

                //pthread_mutex_unlock(&myMutex); /* unlock mutex */
                /* sleep for 1 millisecond so that other threads can run */
                usleep(CONSUMER_SLEEP_TIME);
        }

        return NULL;
}

int main(int argc, char* argv[]) {
        int t1=1, t2=2;
        pthread_t consumers[CONSUMERS], producers[PRODUCERS];
        int i,rc,c[CONSUMERS],p[PRODUCERS];



        sem_init(&semProducer,0,1);
        sem_init(&semConsumer,0,1);
        /* Initialize mutex and condition variable objects */
        //pthread_mutex_init(&myMutex, NULL);
        //pthread_cond_init (&myCV, NULL);


        for(i=0; i<PRODUCERS; i++) {
            p[i]=i;
            rc = pthread_create(&producers[i], NULL, produce, (void *)&p[i]);

            if(rc){
                perror("In producer create method");
                exit(1);
            }
            usleep(20000);
        }

        for(i=0; i<CONSUMERS; i++) {
            c[i]=i;
            rc = pthread_create(&consumers[i], NULL, consume, (void *)&c[i]);

            if(rc){
                perror("In producer create method");
                exit(1);
            }
            usleep(10000);
        }

        usleep(MAIN_SLEEP_TIME);



         /* cancel all threads */
        for(i = 0; i < CONSUMERS; ++i)
            pthread_cancel(consumers[i]);

        for(i = 0; i < PRODUCERS; ++i)
            pthread_cancel(producers[i]);


        sem_destroy(&semConsumer);
        sem_destroy(&semProducer);

        /* Create one consumer thread and two producer threads * /
        pthread_create(&consumer, NULL, consume, NULL);
        pthread_create(&producer1, NULL, produce, (void *)&t1);
        pthread_create(&producer2, NULL, produce, (void *)&t2);

        /* Wait for all threads to complete * /
        pthread_join(consumer, NULL);
        pthread_join(producer1, NULL);
        pthread_join(producer2, NULL);

        /* Clean up and exit * /
        pthread_mutex_destroy(&myMutex);
        pthread_cond_destroy(&myCV);
        pthread_exit(NULL); */
        return 0;
}
