 #include <stdio.h>
 #include <mqueue.h>
 #include <sys/stat.h>
 #include <stdio.h>
 #include <mqueue.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>

 #define QUEUE_NAME "/my_queue"
 #define MAX_MSG_LEN 10000

 int main(int argc, char *argv[]) {
 mqd_t myQ_id;
 char msgcontent[MAX_MSG_LEN];
 int msg_size;
 unsigned int priority;

 /* open the queue created by the sender */
 myQ_id = mq_open(QUEUE_NAME, O_RDWR);
 if (myQ_id == (mqd_t)-1) {
 perror("In mq_open()");
 exit(1);
 }


 /* retreve a message from the queue */
 msg_size = mq_receive(myQ_id, msgcontent, MAX_MSG_LEN, &priority);
 if (msg_size == -1) {
 perror("In mq_receive()");
 exit(1);
 }

 /* output message info */
 printf("Received a message.\n");
 printf(" Content: %s\n", msgcontent);
 printf(" Size: %d bytes.\n", msg_size);
 printf(" Priority: %d\n", priority);

 /* close the qeueu */
 mq_close(myQ_id);

 return 0;
 }
