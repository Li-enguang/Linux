#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define PRO 3 //生产者
#define CON 2 //消费者
#define BUFF_SIZE 5 //仓库容量
 
sem_t full_id; //还有多少个装满仓库
sem_t empty_id; //还有多少个可以消费（仓库变空）
pthread_mutex_t mutex;

pthread_t tid[PRO+CON];
int buff[BUFF_SIZE];
int in=0;
int out=0;
int ready=1; //商品序号
void* pro(void* arg)
{
	int i=*(int *)arg;
	int a=0; // 时间的
	int id=0; //buff的下标
	while(1){
	sem_wait(&full_id); //等待 有位置了才能生产
	pthread_mutex_lock(&mutex);
	printf("\nid : %d pthread : %p 生产中\n",i,pthread_self());
	printf("正在罗列商品序号\n");
	for(a=0;a<=10;a++)
	{
		printf("===");
		fflush(stdout);
		usleep(100000); //1秒 = 100万微秒
	}
	printf("\n");
	buff[in]=ready++;
	for(id=0;id<BUFF_SIZE;id++)
	{
		printf("%d 号商品 : %d",id+1,buff[id]);		
		if(in==id)
		{
			printf("\t 生产完毕\n");		
		}
		else printf("\n");
		}
	sleep(1);
	in=(in+1)%BUFF_SIZE;
	pthread_mutex_unlock(&mutex);
	printf("\nid : %d pthread : %p 生产结束\n",i,pthread_self());
	usleep(500000);
	sem_post(&empty_id); //生产完了 可以通知消费了
	}
	return arg;
}

void* con(void* arg)
{
	int i=*(int *)arg;
	int id;
	int a;
	while(1){
	sem_wait(&empty_id); //等待 有库存才能消费、
	pthread_mutex_lock(&mutex);
	printf("\nid : %d pthread : %p 准备消费\n",i,pthread_self());
	printf("正在罗列商品序号\n");
	for(a=0;a<=10;a++)
	{
		printf("===");
		fflush(stdout);
		usleep(100000); //1秒 = 100万微秒
	}
	printf("\n");
	for(id=0;id<BUFF_SIZE;id++)
	{
		printf("%d 号商品 : %d",id+1,buff[id]);		
		if(out==id)
		{
			printf("\t 已出货\n");		
		}
		else printf("\n");
		}
		sleep(1);
	 out=(out+1)%BUFF_SIZE;
	printf("\nid : %d pthread : %p 消费结束\n",i,pthread_self());
	usleep(500000);
	pthread_mutex_unlock(&mutex);
	sem_post(&full_id); //消费完了 通知生产
	}
	return arg;
}

int main()
{
	int i;
	pthread_mutex_init(&mutex,NULL);
	sem_init(&full_id,0,BUFF_SIZE);
	sem_init(&empty_id,0,0);
	for(i=0;i<PRO;i++)
	{
		int *p=malloc(sizeof(int));
		*p=i;
		pthread_create(&tid[i],NULL,pro,p);
	}
	for(i=0;i<CON;i++)
	{
		int *p=malloc(sizeof(int));
		*p=i;
		pthread_create(&tid[i+PRO],NULL,con,p);
	}

	for(i=0;i<PRO+CON;i++)
	{
		int *p=NULL;
		pthread_join(tid[i],(void**)&p);
		free(p);
	}
	pthread_mutex_destroy(&mutex);
	sem_destroy(&full_id);
	sem_destroy(&empty_id);
}





