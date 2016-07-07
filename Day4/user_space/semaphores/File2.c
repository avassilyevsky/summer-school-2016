#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "bin_sem.h"

int main(int argc,char *argv[])
{
    key_t key = ftok("File1.c",1);
    int semid = binary_semaphore_allocation(key,0666 | IPC_CREAT);
    if(semid >= 0)
    {
	printf("pr2: Trying to take the semaphore\n");
	binary_semaphore_take(semid);
	printf("pr2: Fuck,yeah! Have taken one\n");
	binary_semaphore_free(semid);
	printf("Now the semaphored is released again\n");
	return 0;
    }
    printf("pr2: Unable to get semid\n");
    return 1;
}