 #include <stdio.h>
 #include <mqueue.h>
 #include <sys/stat.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 #include <errno.h>
 #define QUEUE_NAME "/my_queue"
 #define MAX_MSG_LEN 100

 int main(int argc, char *argv[]) {
 mqd_t myQ_id;
 unsigned int msg_priority = 0;
 pid_t my_pid = getpid();
 char msgcontent[MAX_MSG_LEN];

 /* create a message queue */
 myQ_id = mq_open(QUEUE_NAME, O_RDWR | O_CREAT | O_EXCL,
 S_IRWXU | S_IRWXG, NULL);

 /* if not successful */
 if (myQ_id == (mqd_t)-1) {
 /* if the queue already exists, simply open it */
 if (errno == EEXIST){
 myQ_id = mq_open(QUEUE_NAME, O_RDWR );

 if (myQ_id == (mqd_t)-1) {
 perror("In mq_open(2)");
 exit(1);
 }
 }
 else {
 perror("In mq_open(4)");
 exit(1);
 }
 }

 /* compose a message */
 snprintf(msgcontent, MAX_MSG_LEN, "Hello from process %u", my_pid);

 /* send the message */
 if (mq_send(myQ_id, msgcontent, strlen(msgcontent)+1, msg_priority) == 0){
 printf("A message is sent. \n");
 printf(" Content: %s\n", msgcontent);
 }
 else {
 perror("In mq_send()");
 exit(1);
 }

 /* close the queue */
 mq_close(myQ_id);

 return 0;
 }
