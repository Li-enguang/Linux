#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
	
void file_type(mode_t mode)
{
	char buf[11]={"----------"};		
	switch(mode&S_IFMT)
	{
		case S_IFSOCK: buf[0]='s';break;
		case S_IFLNK:  buf[0]='l';break;
		case S_IFREG:  buf[0]='-';break;
		case S_IFBLK:  buf[0]='b';break;
		case S_IFDIR:  buf[0]='d';break;
		case S_IFCHR:  buf[0]='c';break;
		case S_IFIFO:  buf[0]='p';break;
	}
	if(mode&S_IRUSR) buf[1]='r';
	if(mode&S_IWUSR) buf[2]='w';
	if(mode&S_IXUSR) buf[3]='x';
	if(mode&S_IRGRP) buf[4]='r';
	if(mode&S_IWGRP) buf[5]='w';
	if(mode&S_IXGRP) buf[6]='x';
	if(mode&S_IROTH) buf[7]='r';
	if(mode&S_IWOTH) buf[8]='w';
	if(mode&S_IXOTH) buf[9]='x';
	
	printf("%s. ",buf);
}

void file_time(long int time)
{
 	struct tm *ptm;
	ptm=localtime(&time);
	//printf("%s ",asctime(ptm));
	printf("%d月  %d %d: %d ",ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min);

}
void ls_l(char* name)
{
	struct stat st;		
	stat(name,&st);
	file_type(st.st_mode);
	printf("%d ",st.st_nlink);
	struct passwd *pw =getpwuid(st.st_uid);
	printf("%s ",pw->pw_name);
	struct group *gp=getgrgid(st.st_gid);
	printf("%s ",gp->gr_name);
	printf("%d ",(int)st.st_size);
	file_time(st.st_mtime);
	printf("%s ",name);
}
int main(int argc,char* argv)
{
	DIR* pdir=opendir(".");
	struct dirent *pdirent=NULL;
	while((pdirent=readdir(pdir))!=NULL)
	{
		if(pdirent->d_name[0]=='.')
		continue;
		ls_l(pdirent->d_name);
		printf("\n");
	}

	closedir(pdir);
}
