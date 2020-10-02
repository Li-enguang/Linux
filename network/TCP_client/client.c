#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
	int fd=socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(9999);
	inet_aton("192.168.190.5",&addr.sin_addr);
	
	//连接服务器
	connect(fd,(struct sockaddr*)&addr,sizeof(addr));
	printf("客户端连接服务器成功\n");
	char buf[1024];
	while(fgets(buf,1024,stdin)!=NULL)
	{	
		write(fd,buf,strlen(buf));
		memset(buf,0x00,sizeof(buf));
		read(fd,buf,1024);
		printf("%s\n",buf);
    }
}
