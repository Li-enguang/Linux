#include<stdio.h>
#include<unistd.h>
int main()
{

char buf[100]={};
int num=0;

for(num=0;num<=100;num++)
{
buf[num]='#';
printf("[%-100s]%d%%\r",buf,num);
fflush(stdout);
usleep(100000);
}
printf("\nThe progress bar program succeeded\n");
return 0;
}
