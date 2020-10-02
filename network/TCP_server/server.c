#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
void parse(char* buf)
{
	int i;
	for( i=0;i<strlen(buf);i++)
	{
		if(buf[i]>='a'&&buf[i]<='z')
				buf[i]=buf[i]-32;
	}
}

void handler(int s)
{
	while(waitpid(-1,NULL,WNOHANG)!=-1);		
}
int main()
{	
	//信号处理 ： 子进程
	struct sigaction act;
	act.sa_handler=handler;
	act.sa_flags=0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD,&act,NULL);

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9999);
	inet_aton("192.168.190.5",&addr.sin_addr);
	printf("创建服务器\n");
	//创建监听套接字
	int listen_fd=socket(AF_INET,SOCK_STREAM,0);
	//绑定ip和端口
	int bind_fd=bind(listen_fd,(struct sockaddr*)&addr,sizeof(addr));
	if(bind_fd==-1)perror("bind"),exit(-1);
	printf("ip地址绑定完成\n");
	//绑定完成
	//转化为被动套接字
	listen(listen_fd,SOMAXCONN);
	//等待客户端连接
	printf("等待客户端连接\n");
	
	for(;;){
	int newfd =accept(listen_fd,NULL,NULL);
	if(newfd==-1&&errno==EINTR)
	{
		continue;	}
	printf("客户端连接成功\n");
	//开新的进程
	pid_t pid=fork();
	if(pid==0)
		{
		close(listen_fd);
		while(1){
		char buf[1024]={0};
		char pidinfo[1024];
		int r=read(newfd,buf,1024);
		if(r==0)break;
		//printf("来自进程:%s的消息：	",buf[0]);
		printf("%s\n",buf);
		parse(buf);
		write(newfd,buf,strlen(buf));
				}
		close(newfd);
		exit(0);}
	else
		//父进程
		close(newfd);
	}
	close(listen_fd);
}
