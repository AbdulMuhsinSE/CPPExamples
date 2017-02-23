#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define CONSUMERS 5
#define WRITERS 4
#define CONSUMER_SLEEP_TIME 50000
#define PRODUCER_SLEEP_TIME 20000
#define MAIN_SLEEP_TIME 5000000
#define BUFFER_SIZE 4

int theArray[BUFFER_SIZE], size=0;
pthread_mutex_t myMutex;
pthread_cond_t myCV;  /* declare condition variable object */

void *produce(void *arg) {
        int *id = (int *)arg;
        int i;

        for (i = 0; i<5; i++) {
                pthread_mutex_lock(&myMutex); /* lock mutex */

                if (size == BUFFER_SIZE) {
                        printf("Producer %d waiting...\n", id);

                        /* wait for a space to be freed up */
                        pthread_cond_wait(&myCV, &myMutex);
                }

                theArray[size] = i;
                printf("Producer %d added %d.\n", id, i);
                size++;

                if (size == 1) {
                        /* signal consumer to resume */
                        pthread_cond_signal(&myCV);
                }

                pthread_mutex_unlock(&myMutex); /* unlock mutex */

                /* sleep for 1 millisecond, so that other threads can run */
                usleep(1000);
        }
        return NULL;
}

void *consume(void *arg) {
        int item;
        int i;

        for (i=0; i<10; i++) {
                pthread_mutex_lock(&myMutex); /* lock mutex */

                if (size == 0) {
                        printf("Consumer waiting...\n");

                        /* waiting for an item to be added */
                        pthread_cond_wait(&myCV, &myMutex);
                }

                item = theArray[size-1];
                printf("Consumer removed %d.\n", item);
                size--;

                if (size == BUFFER_SIZE-1) {
                        /* signal producer to resume */
                        pthread_cond_signal(&myCV);
                }

                pthread_mutex_unlock(&myMutex); /* unlock mutex */

                /* sleep for 1 millisecond so that other threads can run */
                usleep(1000);
        }

        return NULL;
}

int main(int argc, char* argv[]) {
        int t1=1, t2=2;
        pthread_t consumer, producer1, producer2;

        /* Initialize mutex and condition variable objects */
        pthread_mutex_init(&myMutex, NULL);
        pthread_cond_init (&myCV, NULL);

        /* Create one consumer thread and two producer threads */
        pthread_create(&consumer, NULL, consume, NULL);
        pthread_create(&producer1, NULL, produce, (void *)&t1);
        pthread_create(&producer2, NULL, produce, (void *)&t2);

        /* Wait for all threads to complete */
        pthread_join(consumer, NULL);
        pthread_join(producer1, NULL);
        pthread_join(producer2, NULL);

        /* Clean up and exit */
        pthread_mutex_destroy(&myMutex);
        pthread_cond_destroy(&myCV);
        pthread_exit(NULL);
        return 0;
}
