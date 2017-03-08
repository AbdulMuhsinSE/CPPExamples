#include <sys/mman.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
        const int SHM_SIZE = 1000;
        const int MSG_SIZE = 100;
        const char *name = "/my_shm";
        int *array;
        char message[MSG_SIZE];
        int shm_fd;
        void *ptr;
/* create the shared memory segment */
        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) {
                perror("In shm_open()");
                exit(1);
        }
/* configure the size of the shared memory segment */
        ftruncate(shm_fd,SHM_SIZE);
/* now map the shared memory segment in the address space of the process */
        ptr = mmap(0,SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (ptr == MAP_FAILED) {
                printf("Map failed\n");
                return -1;
        }
/* input a message from keyboard */
        for(int i =0;i<SHM_SIZE;i++){

        }
        printf("Type a message:\n");
        fgets (message, MSG_SIZE, stdin);
/* write the message to the shared memory region */
        sprintf((char *)ptr,"%s",message);
        printf("Your message has been written to the shared memory.\n");
        printf("Content: %s\n", message);
        return 0;
}
