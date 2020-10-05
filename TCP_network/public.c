#include"public.h"

ssize_t readn(int fd,void *buf,size_t count)
{
	int nleft=count;//剩余字节数
	char *p=buf;
	while(nleft>0){
	int r=read(fd,p,nleft);
	if(r==0)
	{
		return count-nleft;//队端关闭，实际读取的字节数
			}
	if(r==-1)
	{	
		if(errno==EINTR)
		continue;
		else return -1;
	}
	nleft-=r;
	p+=r;	
		}
	return count;
}

ssize_t writen(int fd,const void *buf,size_t count)
{
	int nleft=count;//剩余字节数
	const char *p=buf;
	while(nleft>0){
		int r=write(fd,p,nleft);
		if(r==-1)
			if(errno==EINTR)continue;
			return -1;
		nleft-=r;
		p+=r;	
			}
	return count;
}
