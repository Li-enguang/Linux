#include"pthreadpool.h"

void* fun(void* arg)
{	
	int id=*(int*)arg;
	free(arg);
	printf("pthread : %p , task %d begin\n",pthread_self(),id);
	sleep(1);
	printf("pthread : %p , task %d end\n",pthread_self(),id);
}
int main(void)
{
	int i=1;	
	threadpool_t pool;
	thread_init(&pool,3);
	for(i;i<=50;i++)
	{
		int *p=malloc(sizeof(int));
		*p=i;
		threadpool_add_task(&pool,fun,p);		
	}
	//sleep(5);
	threadpool_destroy(&pool);	
	printf("The process is ending\n");
}
