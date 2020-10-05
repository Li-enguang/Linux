#include"public.h"

void handler(int s)
{
	while(waitpid(-1,NULL,WNOHANG)!=-1);		
}
int tcp_init() {
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	// 在TIME_WAIT状态时，可以重新启动服务器
	 int op = 1;
	 setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof op);

	 struct sockaddr_in addr;
	 addr.sin_family = AF_INET;
	 addr.sin_port = htons(1201);
	 addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int r = bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
	 if ( r == -1 ) perror("bind"),exit(1);

	 listen(sfd, SOMAXCONN);
	 return sfd;
 }

int main( void ) {
	 int lfd = tcp_init();
	 
	 struct sigaction act;
	 act.sa_handler=handler;
	 sigemptyset(&act.sa_mask);
	 act.sa_flags=0;
	 sigaction(SIGCHLD,&act,NULL);

	 for(;;){
	 int cfd = accept(lfd, NULL, NULL);
	 if(cfd==-1){continue;printf("11");
	 }
      printf("客户端连接成功\n");
	
	pid_t pid=fork();
	
	if(pid==0){
	close(lfd);
	message_t msg;
	 while ( 1 ) {
	 memset(&msg, 0x00, sizeof msg);

	 if(readn(cfd,&msg.pid,4)<=0)
	 {printf("客户端关闭\n");
	 break;}
	 else
	 printf("来自进程%d的消息：\n",ntohl(msg.pid));
	
	readn(cfd, &msg.len, sizeof(int));
	 int len = ntohl(msg.len);
	printf("定长包头：%d字节\n",len);

	readn(cfd, msg.cont, len);
	printf("数据内容 ：%s\n",msg.cont);
	if ( strncmp(msg.cont, "exit", 4) == 0 )
	 exit(0);
	
	writen(cfd,&msg.pid,sizeof(int));
	 writen(cfd, &msg.len, sizeof(int));
	 writen(cfd, msg.cont, len);
	 }
	 
	}
	else close(cfd);
 }
	close(lfd);
}
