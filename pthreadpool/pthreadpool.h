#ifndef __PTHREADPOOL_H__
#define __PTHREADPOOL_H__

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<errno.h>

typedef struct task //任务队列
{
	void* (*fun)(void *arg);
	void *arg;
	struct task* next;
}task_t;

typedef struct threadpool
{
	pthread_cond_t cond;//条件变量	
	pthread_mutex_t mutex; //互斥变量
	task_t* first;
	task_t* last;
	int counter; //目前线程数
	int idle; //空闲线程数
	int max_pthread;//最大线程数
	int quit;//1 销毁
}threadpool_t;

void thread_init(threadpool_t* pool,int max_pthread); //初始化线程池

void threadpool_add_task(threadpool_t* pool,void*(*fun)(void *),void *arg);//加任务

void threadpool_destroy(threadpool_t* pool); //销毁线程池


#endif // __PTHREADPOOL_H__
