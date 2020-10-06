#include"chat.h"

void* r1(void* s)
{	
	int sfd=*(int*)s;
	char buf[1024];
	while(fgets(buf,1024,stdin)!=NULL)//发送昵称
	{
		write(sfd,buf,strlen(buf));
		fflush(stdout);
	}
}

void* r2(void* s)
{
	int sfd=*(int*)s;
	//发送完毕，开始接收数据
	char buf[1024];
	while(1){
	memset(&buf,0x00,sizeof(buf));
	if(read(sfd,buf,sizeof(buf))<=0)
		{	printf("服务端关闭\n");break;}
		printf("%s\n",buf);
		fflush(stdout);
	}
	exit(0);
}
void handler(int s)
{
	printf("管道破裂\n");
}
int main()
{
	//捕获管道破裂的信号
	struct sigaction act;
	act.sa_handler=handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGPIPE,&act,NULL);
	
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(1201);
	inet_aton("192.168.190.5",&addr.sin_addr);

	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))==-1)
			perror("connect"),exit(-1);

	pthread_t t1,t2;
	int*p=malloc(sizeof(4));
	*p=sfd;
	pthread_create(&t1,NULL,r1,p);
	pthread_create(&t2,NULL,r2,p);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	free(p);
}
