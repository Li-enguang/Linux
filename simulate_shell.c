#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
#include<unistd.h>

void do_shell(int argc,char* argv[])
{	
	/*int i;
	for( i=0;i<argc;i++)
	{
		printf("%s\n",argv[i]);
	}*/
	pid_t pid;
	pid=fork();
	int status;
	int ret;
	switch(pid)
	{
		case -1 :
		printf("errno !\n");
			break;
		case 0 :
		ret=execvp(argv[0],argv);
		printf("command not found!\n");
			break;
		default :
		while(waitpid(pid,&status,WNOHANG)!=pid)
		;	break;
	}

}

void  parse( char* str)
{	
  
  int argc=0;
  char* argv[10]={0};
  int status=0;
	int i=0;
	for(i;str[i]!='\0';i++)
	{
		if(status==0&&!isspace(str[i]))
		{
			argv[argc]=str+i;	
			argc++;
			status=1;
		}
		if(status==1&&isspace(str[i]))
		{
			status=0;
			str[i]='\0';
		}
	
	}
	
	argv[argc]=NULL;
	do_shell(argc,argv);
		
}

void getstr(char* str)
{		
	printf("[simulate shell]:");
	if(scanf("%[^\n]%*c",str)!=1)
	{
		int  c=getchar();		
	}
}

int main()
{	
	printf("simulating terminal\n");
	while(1)
	{	
		char str[50];	
		memset(str,0x00,50);
		getstr(str);
	if(strcmp(str,"exit")==0)
	{	
		return;
	}
		parse(str);
	}
	return 0;		
}
