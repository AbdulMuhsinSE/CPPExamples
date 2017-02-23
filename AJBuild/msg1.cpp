#include <stdio.h>
 #include <mqueue.h>
 #include <sys/stat.h>
 #include <stdio.h>
 #include <mqueue.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 #include <errno.h>
 #include <iostream>

 #define QUEUE_NAME "/my_queue"
 #define MAX_MSG_LEN 100

int main(int argc, char *argv[]) {
        mqd_t myQ_id;
        unsigned int msg_priority = 0;
        pid_t my_pid = getpid();
        char msgcontent[MAX_MSG_LEN];
        char *input;
        char *stop ="stop";
        /* create a message queue */

        printf("User input:");
        scanf("%s", input);
        while (strcmp(input, stop)!=0) {

                printf("%s",input);

                myQ_id = mq_open(QUEUE_NAME, O_RDWR | O_CREAT | O_EXCL,
                         S_IRWXU | S_IRWXG, NULL);
                /* if not successful */
                if (myQ_id == (mqd_t)-1) {
                        /* if the queue already exists, simply open it */
                        if (errno == EEXIST) {
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
                snprintf(msgcontent, MAX_MSG_LEN,input,my_pid );

                /* send the message */
                if (mq_send(myQ_id, msgcontent, strlen(msgcontent)+1, msg_priority) == 0) {
                        printf("A message is sent. \n");
                        printf(" Content: %s\n", msgcontent);
                }
                else {
                        perror("In mq_send()");
                        exit(1);
                }

                scanf("%s", input);
        }

        /* close the queue */
        mq_close(myQ_id);

        return 0;
        //DO NUMBER 13
}
