#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{
    int segment_id;
    char* shared_memory;
    struct shmid_ds shmbuffer;
    const int shared_segment_size = 1000;

    segment_id = shmget (IPC_PRIVATE, shared_segment_size,
			IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);


    shared_memory = (char*) shmat (segment_id, 0, 0);
    printf ("shared memory attached at address %p\n", shared_memory);

    sprintf(shared_memory,"C ya l8r, a11i980r");
    printf("%s\n",shared_memory);

    shmdt(shared_memory);
    return 0;

}