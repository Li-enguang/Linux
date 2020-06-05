#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

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

int main(int argc,char* argv[],char* envp[])
{

env_init(argc,argv,envp);

/*while(*argv)
  {
     *argv=NULL;
     *argv++;
     printf("%d %s\n",i++,*argv);
  }
while(*envp)
  {
     *envp=NULL;
    *envp++;
     printf("%d %s\n",i++,*envp);
  }*/
pid_t pid=fork();

switch(pid) {
  case 0:
          strcpy(argv[0],"worker");
          printf("child\n");
          break;
  default:
          strcpy(argv[0],"master");
          printf("partents");
          break;
  }

sleep(1);
getchar();
/*char* bash=getenv("SHELL");
printf("%s\n",bash);
putenv("AAA=abc");
printf("%s\n",getenv("AAA"));
*/
return 0;
}
