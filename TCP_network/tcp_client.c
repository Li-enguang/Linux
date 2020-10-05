#include"public.h"

void* r1(void* s)
{	
	int sfd=*(int*)s;
	message_t msg;
	while(fgets(msg.cont,1024,stdin)!=NULL)
	{
		msg.len=htonl(strlen(msg.cont));
		msg.pid=htonl(getpid());
		writen(sfd,&msg.pid,4);
		writen(sfd,&msg.len,sizeof(int));
		writen(sfd,msg.cont,strlen(msg.cont));
	}
}

void* r2(void* s)
{
	int sfd=*(int*)s;
	//发送完毕，开始接收数据
	message_t msg;
	memset(&msg,0x00,sizeof(msg));
	while(1){
	if(readn(sfd,&msg.pid,4)<=0)
		{	printf("服务端关闭\n");break;}
	printf("当前进程号 ：%d\n",ntohl(msg.pid));
	readn(sfd,&msg.len,4);
	printf("定长包头：len = %d\n",ntohl(msg.len));

	readn(sfd,msg.cont,ntohl(msg.len));
	printf("数据内容：%s\n",msg.cont);	
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
	printf("连接成功\n");

	pthread_t t1,t2;
	int*p=malloc(sizeof(4));
	*p=sfd;
	pthread_create(&t1,NULL,r1,p);
	pthread_create(&t2,NULL,r2,p);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	free(p);
}
