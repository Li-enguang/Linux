#include "select.hpp"
int main()
{
    TcpSocket lst_sock;//监听套接字
    //1. 创建套接字
    CHECK_RET(lst_sock.Socket());
    //2. 绑定地址信息
    CHECK_RET(lst_sock.Bind("0.0.0.0", 9000));
    //3. 开始监听
    CHECK_RET(lst_sock.Listen(1));

    Select s;
    s.Add(lst_sock);
    while(1) {
        std::vector<TcpSocket> list;
        bool ret = s.Wait(&list);
        if (ret == false) {
            return -1;
        }
        for (auto sock : list) {
            if (sock.GetFd() == lst_sock.GetFd()) {
                TcpSocket con_sock;//与指定客户端通信的套接字
                std::string cli_ip;
                int cli_port;
                ret=lst_sock.Accept(&con_sock);
                if (ret == false) {
                    continue;
                }
                s.Add(con_sock);
            }else {
                std::string buf;
                ret = sock.Recv(&buf);
                if (ret == false) {
                    sock.Close();
                    s.Del(sock);
                    continue;
                }
                printf("client say:%s\n",buf.c_str());
                std::cout << "server say:";
                std::cin >> buf;
                ret = sock.Send(buf);
                if (ret == false) {
                    sock.Close();
                    s.Del(sock);
                }
            }
        }
    }
    //6. 关闭套接字
    lst_sock.Close();
    return 0;
}
