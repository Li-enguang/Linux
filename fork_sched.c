#define _GNU_SOURCE

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sched.h>

int g_argc;
char* g_argv[10];
char* g_envp[500];
int i=0;
void env_init(int argc,char* argv[],char* envp[])//环境变量的转移
{

g_argc=argc;
for(i=0;argv[i]!=NULL;i++)
{
g_argv[i]=argv[i];
printf("%d %s\n",i,g_argv[i]);
}

for(i;envp[i]!=NULL;i++)
  {
  g_envp[i]=envp[i];
  printf("%d %s\n",i,g_envp[i]);
  }

}

void process_mange(int choice)//把该进程绑定到指定cpu
{
	cpu_set_t cpu_affinity;//定义一个cpu变量
	CPU_ZERO(&cpu_affinity);//cpu初始化
	CPU_SET(choice,&cpu_affinity);//核的绑定 cat /proc/cpuinfo 
	sched_setaffinity(0,sizeof(cpu_set_t),&cpu_affinity);//关键的绑定操作 

}  

int main(int argc,char* argv[],char* envp[])
{
int choice=0; 
//env_init(argc,argv,envp);//转移环境变量
pid_t pid=fork();

switch(pid) {
  case 0:
          strcpy(argv[0],"worker");
          process_mange(0); //子进程绑定到0核
		  while(1)
		  {		  
		  printf("child\n");
		  sleep(1); 
		  } 
          break;
  default:
          strcpy(argv[0],"master");
          process_mange(1);//父进程绑定到1核 
		  while(1)
		  {
		  printf("partents\n");
		  sleep(1); 
		  } 
          break;
  		}
	return 0;
}
