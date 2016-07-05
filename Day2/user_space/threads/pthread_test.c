#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS	5
#define NUM_ITER	20000

static int	a_var=0;

void	*add_func(void *param)
{
    int i;
    for(i=0;i<NUM_ITER;i++)
    {
	a_var++;
	printf("a_var incremented. a_var=%d\n",a_var);
	usleep(1);
    }
}

void	*sub_func(void *param)
{
    int i;
    for(i=0;i<NUM_ITER;i++)
    {
	a_var--;
	printf("a_var decremented. a_var=%d\n",a_var);
	usleep(1);
    }
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;
	rc=pthread_create(&threads[0],NULL,add_func,(void*)t);
	if (rc)
	{
		printf("ERROR: return code from pthread_create(,,add_func) is %d\n",rc);
		exit(-1);
	}
	rc=pthread_create(&threads[1],NULL,sub_func,(void*)t);
	if (rc)
	{
		printf("ERROR: return code from pthread_create(,,sub_func) is %d\n",rc);
		exit(-1);
	}

	sleep(10);
	for(t=0; t<2;t++)
	{
		//pthread_cancel(threads[t]);
		pthread_join(threads[t],NULL);
		printf("Thread #%ld finished\n",t);
	}

	return 0;
}
