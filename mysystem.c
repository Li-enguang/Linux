#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int mysystem(const char* cmd)
{ 
	int status;
	pid_t pid=fork();
	if(pid==-1) exit(127);
	if(pid==0)
	{	//printf("child\n");
		printf("%s ",cmd);
		execlp("/bin/sh","/bin/sh","-c",cmd,NULL);
		exit(0);
		}
	else
	{
		//sleep(1);	
		//printf("partent\n");
		while(waitpid(pid,&status,WNOHANG)!=pid)
		{
		printf("waiting - - - -\n");
		sleep(1);
		}
		printf("waitpid success!\n");
	}
	if(WIFEXITED(status))//子进程正常退出，返回真
	{	
		printf("child exit: %d",WEXITSTATUS(status));//获取子进程的退出码	
		return WEXITSTATUS(status);
			}
	else return -1; //非正常退出，返回-1
}

int main()
{	
	char command[]={0};
	printf("please put cmd:\n");
	scanf("%[^\n]",command);
	mysystem(command);	
	return 0;	
}
