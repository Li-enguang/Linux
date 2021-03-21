#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void setnonblock(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}
int main (int argc, char *argv[])
{
    setnonblock(STDIN_FILENO);
    while(1) {
        char buf[1024] = {0};
        //文件描述符：0-输入；1-输出；2-错误输出
        //文件流指针：stdin-输入；stdout-输出；stderr-标准错误
        int ret;
        while((ret = read(STDIN_FILENO, buf, 1023)) < 0) {
            if (EAGAIN == errno || errno == EINTR) {
                printf("读了一下没有数据，休息一下\n");
                sleep(2);
                continue;
            }
            return -1;//如果错误不是因为没有数据
        }
        printf("%d\n", ret);

        printf("input:%s\n", buf);
    }
    return 0;
}
