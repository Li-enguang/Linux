#include <iostream>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CHECK_RET(q) if((q)==false){return -1;}
using namespace std;

class TcpSocket
{
    public:
        TcpSocket():_sockfd(-1){}
        int GetFd() {
            return _sockfd;
        }
        void SetFd(int fd) {
            _sockfd = fd;
            return ;
        }
        bool Socket() {
            _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            if (_sockfd < 0) {
                perror("socket error");
                return false;
            }
            return true;
        }
        bool Bind(const string &ip, const int port) {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = bind(_sockfd, (struct sockaddr*)&addr, 
                    len);
            if (ret < 0) {
                perror("bind error");
                return false;
            }
            return true;
        }
        bool Listen(int backlog = 5) {
            //int listen(int sockfd,  backlog);
            int ret = listen(_sockfd, backlog);
            if (ret < 0) {
                perror("listen error");
                return false;
            }
            return true;
        }
        bool Connect(const string &ip, const int port) {
            //int connect(int sockfd, sockaddr *srvaddr, len)
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            socklen_t len = sizeof(struct sockaddr_in);
            int ret = connect(_sockfd, 
                    (struct sockaddr*)&addr, len);
            if (ret < 0) {
                perror("connect error");
                return false;
            }
            return true;
        }
        bool Accept(TcpSocket *sock, string *ip = NULL, 
                int *port = NULL) {
            //int accept(int sockfd, sockaddr* addr, int *len)
            struct sockaddr_in cli_addr;
            socklen_t len = sizeof(struct sockaddr_in);
            int fd = accept(_sockfd, 
                    (struct sockaddr*)&cli_addr, &len);
            if (fd < 0) {
                perror("accept error");
                return false;
            }
            //fd是用于与cli_addr地址的客户端进行通信的
            //通过传入的sock返回给外部，
            //外部通过sock与客户端进行通信
            sock->_sockfd = fd;
            if (ip != NULL) {
                *ip = inet_ntoa(cli_addr.sin_addr);
            }
            if (port != NULL) {
                *port = ntohs(cli_addr.sin_port);
            }
            return true;
        }
        bool Recv(string *buf) {
            //ssize_t recv(int sockfd, char *buf,int len,flag)
            //返回值：错误返回-1；连接断开返回0；
            //成功返回实际接收到的数据长度
            char tmp[4096] = {0};
            int ret = recv(_sockfd, tmp, 4096, 0);
            if (ret < 0) {
                perror("recv error");
                return false;
            }else if (ret == 0) {
                printf("connect shutdown");
                return false;
            }
            buf->assign(tmp, ret);
            return true;
        }
        bool Send(const string &data) {
            //ssize_t send(int sockfd, char *data, int len, f)
            int total_len = 0;//实际发送的数据长度
            while(total_len < data.size()){
                int ret = send(_sockfd, 
                        data.c_str() + total_len,
                        data.size() - total_len, 0);
                if (ret < 0) {
                    perror("send error");
                    return false;
                }
                total_len += ret;
            }
            return true;
        }
        bool Close() {
            if (_sockfd < 0) {
                close(_sockfd);
                _sockfd = -1;
            }
            return true;
        }
    private:
        int _sockfd;
};
