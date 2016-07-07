#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "bin_sem.h"

int main(int argc,char *argv[])
{
    key_t key = ftok("File1.c",1);
    int semid = binary_semaphore_allocation(key, 0666 | IPC_CREAT);
    if (semid<0)
    {
	printf("Oops!\n");
	return 1;
    }
    binary_semaphore_initialize(semid);
    binary_semaphore_take(semid);
    printf("pr1: proc1 took the semaphore\n");
    sleep(20);
    binary_semaphore_free(semid);
    printf("pr1: proc1 released the semaphore\n");
    return 0;
}