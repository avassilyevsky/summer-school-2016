#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

int binary_semaphore_allocation (key_t key, int sem_flags);
int binary_semaphore_deallocation (int semid);
int binary_semaphore_initialize (int semid);
int binary_semaphore_take (int semid);
int binary_semaphore_free (int semid);