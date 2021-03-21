#include<poll.h> #include<vector>
#include <sys/socket.h>
#include"tcpsocket.hpp"
#define MAX_SIZE 10

using namespace std;



int main(int argc, char* argv[])
{
    if(argc != 3)
    {   
        cerr << "正确输入方式: ./select_srv.cc ip port\n" << endl;
        return -1; 

    } 

    string srv_ip = argv[1];
    uint16_t srv_port = stoi(argv[2]);

    TcpSocket lst_socket;
    //创建监听套接字
    lst_socket.Socket();
    CHECK_RET(lst_socket.Socket());
    //绑定地址信息
    CHECK_RET(lst_socket.Bind(srv_ip, srv_port));
    //开始监听
    CHECK_RET(lst_socket.Listen());

    struct pollfd poll_fd[MAX_SIZE];

    poll_fd[0].fd = lst_socket.GetFd();
    poll_fd[0].events = POLLIN;

    int i = 0, maxi = 0;
    for(i = 1; i < MAX_SIZE; i++)
    {
        poll_fd[i].fd = -1;
    }

    while(1)
    {
        int ret = poll(poll_fd, maxi + 1, 2000);

        if(ret < 0)
        {
            cerr << "not ready" << endl;
            continue;
        }
        else if(ret == 0)
        {
            cerr << "wait timeout" << endl;
            continue;
        }


        //监听套接字就绪则增加新连接
        if(poll_fd[0].revents & (POLLIN | POLLERR))
        {
            struct sockaddr_in addr;
            socklen_t len = sizeof(sockaddr_in);
            //创建一个新的套接字与客户端建立连接
            int new_fd = accept(lst_socket.GetFd(), (sockaddr*)&addr, &len);

            for(i = 1; i < MAX_SIZE; i++)
            {
                if(poll_fd[i].fd == -1)
                {
                    poll_fd[i].fd = new_fd;
                    poll_fd[i].events = POLLIN;
                    break;
                }
            }

            if(i > maxi)
            {
                maxi = i;
            }

            if(--ret <= 0)
            {
                continue;
            }
        }

        for(i = 1; i <= maxi; i++)
        {   
            if(poll_fd[i].fd == -1)
            {
                continue;
            }

            if(poll_fd[i].revents & (POLLIN | POLLERR))
            {
                //新数据到来
                char buff[4096] = { 0  };
                int ret = recv(poll_fd[i].fd, buff, 4096, 0); 

                if(ret == 0)
                {   
                    std::cerr << "connect error" << std::endl;

                    close(poll_fd[i].fd);
                    poll_fd[i].fd = -1;
                }   
                else if(ret < 0)
                {   
                    std::cerr << "recv error" << std::endl;
                    close(poll_fd[i].fd);
                    poll_fd[i].fd = -1;
                } 
                else
                {
                    cout << "cli send message: " << buff << endl;
                }

                if(--ret <= 0)
                {
                    break;
                }
            }
        }
    }
    lst_socket.Close();
    return 0;
}
