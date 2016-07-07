#include	<stdio.h>
#include	<stdlib.h>
#include	<pthread.h>

#define	THREADS_NUM	5

typedef struct TParam
{
    int	thread_n;
}TParam,*PParam;

int global_counter=1;
int threads_waiting=0;
pthread_mutex_t mtx1=PTHREAD_MUTEX_INITIALIZER,
		mtx2=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond;

void* worker_thread(void *param)
{
    PParam pp=(PParam)param;
    printf("thread %d started\n",pp->thread_n);
    pthread_mutex_lock(&mtx1);
    global_counter++;
    pthread_mutex_unlock(&mtx1);
    sleep(global_counter);
    pthread_mutex_lock(&mtx1);
    threads_waiting++;
    pthread_mutex_unlock(&mtx1);

    printf("thread %d suspended\n",pp->thread_n);
    if (threads_waiting>=THREADS_NUM)
    {
	printf("last thread, global_counter=%d\n",global_counter);
	pthread_cond_broadcast(&cond);
    }
    else
    {
	pthread_mutex_lock(&mtx2);
	pthread_cond_wait(&cond,&mtx2);
	pthread_mutex_unlock(&mtx2);
    }
    printf("thread %d done\n",pp->thread_n);
};


int main(int argc, char *argv[])
{

    pthread_t threads[THREADS_NUM];
    TParam	t_param[THREADS_NUM];
    int res,i,t;

    pthread_cond_init(&cond,NULL);

    for(i=0;i<THREADS_NUM;i++)
    {
	t_param[i].thread_n=i;
	res=pthread_create(&threads[i],NULL,worker_thread,(void*)&t_param[i]);
	if (res)
	{
	    printf("Error creating thread %d\n",i);

	}
    }
    for(i=0;i<THREADS_NUM;i++)
	pthread_join(threads[i],NULL);
    sleep(2);
}