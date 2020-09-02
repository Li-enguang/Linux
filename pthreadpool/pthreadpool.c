#include"pthreadpool.h"

void thread_init(threadpool_t* pool,int max_pthread)//初始化线程池
{
	pthread_cond_init(&pool->cond,NULL);
	pthread_mutex_init(&pool->mutex,NULL);
	pool->first=NULL;
	pool->last=NULL;
	pool->counter =0;
	pool->idle=0;
	pool->max_pthread =max_pthread;
	pool->quit=0;
}
void* route(void* arg)
{
	threadpool_t* pool=(threadpool_t*)arg;
	printf("pthread : %p created \n",pthread_self());
	while(1)
	{	
		int timeout=0;	
		pthread_mutex_lock(&pool->mutex);
		pool->idle++; //刚开始是空闲线程	
		while(pool->first==NULL&& pool->quit==0)			
		{	
			struct timespec abstime;
			clock_gettime(CLOCK_REALTIME,&abstime);
			abstime.tv_sec+=2;
			int ret=pthread_cond_timedwait(&pool->cond,&pool->mutex,&abstime); //带时间的等待线程函数	
			if(ret==ETIMEDOUT)
			{
				timeout=1;
				printf("pthread : %p timeout\n",pthread_self());
				break;
					}
		}
		pool->idle--;//开始工作了	
	
		if(pool->first!=NULL) //有任务，执行回调函数
		{
		task_t* tmp=pool->first;
		pool->first=tmp->next;
		pthread_mutex_unlock(&pool->mutex);
		tmp->fun(tmp->arg);
		free(tmp);
		pthread_mutex_lock(&pool->mutex);
		}
		
		if(pool->first==NULL&& timeout==1) //超时了，结束线程
		{
			pool->counter--;	
			pthread_mutex_unlock(&pool->mutex);
			break;	
				}
		if(pool->first==NULL&&pool->quit==1) 
		{
			pool->counter--;
			if(pool->counter==0)//如果最后一个任务执行完了，发通知，告诉销毁函数
			{
				pthread_cond_signal(&pool->cond);		
			}
			pthread_mutex_unlock(&pool->mutex);
			break;
		}
		
	pthread_mutex_unlock(&pool->mutex);
	}
	printf("pthread : %p exit \n",pthread_self());
}


void threadpool_add_task(threadpool_t* pool,void*(*fun)(void *),void *arg)//加任务
{
	task_t * new_task=malloc(sizeof(task_t));
	new_task->fun=fun;
	new_task->arg=arg;

	pthread_mutex_lock(&pool->mutex);//往任务队列中加任务
	
	if(pool->first==NULL){
		pool->first=new_task;}
	else {
		pool->last->next=new_task;
	}
	pool->last=new_task;

	//如果线程池有空闲线程，唤醒空闲线程，执行当前任务
	if(pool->idle>0)
	{
		pthread_cond_signal(&pool->cond);
			}
	//如果没有空闲线程，创建新线程
	else if(pool->counter < pool->max_pthread)
	{	
		pthread_t tid;
		pthread_create(&tid,NULL,route,pool);
		pool->counter++;	
	}
	pthread_mutex_unlock(&pool->mutex);
}

void threadpool_destroy(threadpool_t* pool) //销毁线程池
{
	if(pool->quit==1)
			return;
	pthread_mutex_lock(&pool->mutex);	
		pool->quit=1;
		if(pool->counter>0)//要有进行中的线程
		{
			if(pool->idle>0) //并且有空闲线程
			{
				pthread_cond_broadcast(&pool->cond); //可以给所有等待中的线程发通知 ，结束	
			}
			while(pool->counter>0) //此时若有线程还在执行任务，等待
				{
					pthread_cond_wait(&pool->cond,&pool->mutex);		
				}
		}
	pthread_mutex_unlock(&pool->mutex);	
	pthread_cond_destroy(&pool->cond);
	pthread_mutex_destroy(&pool->mutex);
}
