#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<termios.h>

typedef void(*handle)(int s);
void a(int s)
{	
	struct winsize w;
	ioctl(0,TIOCGWINSZ,&w);
	printf("row=%d col=%d\n",w.ws_row,w.ws_col);	

}
int main()
{
	handle old=NULL;
	signal(SIGWINCH,a);
	while(1)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}

	return 0;
}

