#include <sys/mman.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

struct sigaction old_action;
const char *name = "/my_shm";

void sigint_handler(int sig_no)
{
    printf("\nCTRL-C pressed\nGetting rid of the shared memory segment\n");
    if (shm_unlink(name) == -1) {
                perror("in shm_unlink()");
                exit(1);
    }
    sigaction(SIGINT, &old_action, NULL);
    kill(0, SIGINT);
}


int main()
{

         struct sigaction action;
        memset(&action, 0, sizeof(action));
        action.sa_handler = &sigint_handler;
        sigaction(SIGINT, &action, &old_action);


        //const char *name = "/my_shm";

        const int SIZE = 1000*4096;
        int shm_fd;

        void *ptr;
/* open the shared memory segment */
        shm_fd = shm_open(name, O_RDONLY, 0666);
        if (shm_fd == -1) {
                perror("in shm_open()");
                exit(1);
        }
/* now map the shared memory segment in the address space of the process */

/* now read from the shared memory region */
        while(1)
        {
            int count = 0;
            ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
            if (ptr == MAP_FAILED) {
                perror("in mmap()");
                exit(1);
            }

            for(int i = 0; i<1000; i++)
            {
                int x = (int)(((char *)ptr)[i*sizeof(int)]);
                if(x != 0)
                {
                    count++;
                }
            }


            printf("Number of elements in shared memory: %d \n", count);
            usleep(200000);

        }
        return 0;
}
