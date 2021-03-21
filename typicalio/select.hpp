#include <iostream>
#include <vector>
#include <sys/select.h>
#include <string.h>
#include "tcpsocket.hpp"

class Select
{
    public:
        Select():_max_fd(-1){
            FD_ZERO(&_rfds);
        }
        bool Add(TcpSocket &sock) {
            int fd = sock.GetFd();
            FD_SET(fd, &_rfds);
            _max_fd = _max_fd > fd ? _max_fd : fd;
            return true;
        }
        bool Del(TcpSocket &sock) {
            int fd = sock.GetFd();
            FD_CLR(fd, &_rfds);
            if (fd != _max_fd) {
                return true;
            }
            for (int i = _max_fd; i >= 0; i--) {
                if (FD_ISSET(i, &_rfds)) {
                    _max_fd = i;
                    break;
                }
            }
            return true;
        }
        bool Wait(std::vector<TcpSocket> *list) {
            struct timeval tv;
            tv.tv_sec = 3;
            tv.tv_usec = 0;
            fd_set tmp_fds;
            memcpy(&tmp_fds, &_rfds, sizeof(fd_set));
            int ret;
            //select(最大描述符+1，读，写，异常，超时时间)
            ret=select(_max_fd+1, &tmp_fds, NULL, NULL, &tv);
            if (ret < 0) {
                perror("select error");
                list->clear();
                return false;
            }else if (ret == 0) {
                printf("select timeout\n");
                list->clear();
                return true;
            }
            for (int i = 0; i <= _max_fd; i++) {
                if (FD_ISSET(i, &tmp_fds)) {
                    TcpSocket sock;
                    sock.SetFd(i);
                    list->push_back(sock);
                }
            }
            return true;
        }
    private:
        int _max_fd;
        fd_set _rfds;
};
