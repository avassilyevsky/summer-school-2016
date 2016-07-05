#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NAME_MAX_LEN	16
#define NUM_THREADS	5
#define NUM_ITER	20000
#define DELTA_SPAN	10


typedef struct  TParams {
    pthread_t thread_id;
    int thread_name_idx,
    iter_count,
    delta

}TParams,*PParams;

static int	a_var=0;

char Names[NUM_THREADS][NAME_MAX_LEN]={"A Thread","Another Thread","Third Thread","Thread 4","5th"};

void	*thread_func(void *params)
{
    int i;
    PParams pp=(PParams)params;

    for(i=0;i<pp->iter_count;i++)
    {
	a_var+=pp->delta;
	printf("[%d - %s] fired, iteration %d/%d, delta:%d: %d\n",
		pp->thread_id,
        Names[pp->thread_name_idx],
		i+1,
		pp->iter_count,
		pp->delta,
		a_var);
	usleep(1);
    }
};


int main(int argc, char *argv[])
{
	TParams threads[NUM_THREADS];
	int rc,i;
	srandom(13578);
	for (i=0;i<NUM_THREADS;i++)
	{
	    threads[i].thread_name_idx=i;
	    threads[i].iter_count=random()%21;
	    threads[i].delta=(random()%DELTA_SPAN)-(DELTA_SPAN/2);
	    rc=pthread_create(&threads[i].thread_id,NULL,thread_func,(void*)&threads[i]);
	    if (rc)
	    {
		printf("ERROR: return code from pthread_create(,,add_func) is %d\n",rc);
		exit(-1);
	    }
	}
	sleep(10);
	for(i=0; i<NUM_THREADS;i++)
	{
		pthread_join(threads[i].thread_id,NULL);
		printf("Thread [%s] finished\n",Names[threads[i].thread_name_idx]);
	}

	return 0;
}
