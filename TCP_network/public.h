#ifndef __PUBLIC__H
#define __PUBLIC__H

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<pthread.h>
#include<sys/wait.h>

typedef struct//消息的格式
{
	int pid;
	int len;//网络字节序的定长部分，
	char cont[1024];//存放数据，变长的部分
}message_t;

ssize_t readn(int fd,void* buf,size_t count);
ssize_t writen(int fd,const void* buf,size_t count);
#endif //__PUBLIC__H
