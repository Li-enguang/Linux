#ifndef __CHAT__H
#define __CHAT__H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<error.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>

typedef	struct client
{
	int cfd;
	char nickname[32];
}client_t;

typedef struct node
{
	client_t cli;
	struct node* next;
	struct node* prev;
}node_t;

#endif //__CHAT__H
