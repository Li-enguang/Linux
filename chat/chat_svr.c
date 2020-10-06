#include"chat.h"

node_t *head=NULL;

void list_init()
{
	node_t* p=malloc(sizeof(node_t));
	p->next=p->prev=p;
	head=p;
}

void list_insert(client_t* cli)//头插
{
	node_t* p=malloc(sizeof(node_t));
	p->cli=*cli;
	p->next=head->next;
	p->prev=head;
	head->next->prev=p;
	head->next=p;
}
void list_erase(int cfd)
{
	node_t *p=head->next; //遍历
	while(p!=head)
	{
		if(p->cli.cfd==cfd)
		{
			p->next->prev=p->prev;
			p->prev->next=p->next;
			free(p);
			break;
		}
		p=p->next;
	}
}

int list_len()
{
	node_t* p=head->next;
	int i=0;
	while(p!=head)
	{
		i++;
		p=p->next;
	}
	return i;
}

void send_all(char* msg)
{
	node_t* p=head->next;
	while(p!=head)
	{
		write(p->cli.cfd,msg,strlen(msg));
		p=p->next;
	}
}

int tcp_init()
{
	int lfd=socket(AF_INET,SOCK_STREAM,0);
	int op=1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port=htons(1201);

	int r=bind(lfd,(struct sockaddr*)&addr,sizeof(addr));
	if(r==-1)perror("bind"),exit(-1);

	listen(lfd,SOMAXCONN);
	return lfd;
}

void* process(void* args)
{
	int cfd=*(int*)args;
	free(args);
	char buf[1024];
	sprintf(buf,"你已连接到本聊天服务器\n");
	write(cfd,buf,strlen(buf));
	sprintf(buf,"请输入您的昵称:");
	write(cfd,buf,strlen(buf));
	char nickname[100];
	read(cfd,nickname,100);
	nickname[strlen(nickname)-1]=0;
	sprintf(buf,"欢迎%s加入聊天室\n",nickname);
	send_all(buf);
	printf("%s",buf);
	client_t cli;
	cli.cfd=cfd;
	sprintf(cli.nickname,"%s",nickname);
	list_insert(&cli);//把客户端拉到链表里
	printf("当前在线人数:%d\n\n",list_len());

	//读取客户端的信息
	while(1)
	{
		memset(buf,0x00,sizeof(buf));
		int r=read(cfd,buf,1024);
		if(r<=0)
		{
			sprintf(buf,"%s退出\n",cli.nickname);
			send_all(buf);
			printf("%s",buf);
			break;
		}
		else{
		time_t timep;
		struct tm*p;
		time(&timep);
		p=localtime(&timep);

		char msg[1024];
		sprintf(msg,"\n%d/%d/%d %d:%d:%d\n%s :",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,cli.nickname);
		send_all(msg);
		send_all(buf);
		}
	}
	list_erase(cfd);	
	printf("当前在线人数:%d\n\n",list_len());
}
int main()
{
	list_init();
	int lfd=tcp_init();
	printf("聊天服务器已开启\n");
	for(;;)//一旦有客户端连接，就创建新的线程
	{
		int cfd=accept(lfd,NULL,NULL);
		pthread_t pid;
		int* p=malloc(sizeof(int));
		*p=cfd;
		pthread_create(&pid,NULL,process,p);
		pthread_detach(pid);
	}
}
