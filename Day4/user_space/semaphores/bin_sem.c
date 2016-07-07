#include "bin_sem.h"

int binary_semaphore_allocation(key_t key, int sem_flags)
{
    return semget(key,1,sem_flags);
}

int binary_semaphore_deallocation (int semid)
{
    union semun ignored_argument;
    return semctl(semid,1,IPC_RMID,0);
}

int binary_semaphore_initialize (int semid)
{
    union semun argument;
    argument.val = 1;
    return semctl(semid, 0, SETVAL, argument);
}

int binary_semaphore_take (int semid)
{
    struct sembuf sem_b;
    sem_b.sem_num =0;
    sem_b.sem_op = -1;
    return semop(semid, &sem_b,1);
}

int binary_semaphore_free (int semid)
{
    struct sembuf sem_b;
    sem_b.sem_num =0;
    sem_b.sem_op = 1;
    return semop(semid, &sem_b,1);
}
