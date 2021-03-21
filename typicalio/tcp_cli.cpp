#include "tcpsocket.hpp"

int main()
{
    TcpSocket cli_sock;
    //1. 创建套接字
    CHECK_RET(cli_sock.Socket());
    //2. 绑定地址信息--但是并不推荐
    //3. 向服务端发起连接请求
    CHECK_RET(cli_sock.Connect("172.31.43.144", 9000));
    while(1) {
        //发送数据
        std::string buf;
        std::cout << "client say:";
        std::cin >> buf;
        CHECK_RET(cli_sock.Send(buf));
        //接收数据
        buf.clear();
        CHECK_RET(cli_sock.Recv(&buf));
        std::cout << "server say: " << buf << std::endl;
    }
    //关闭套接字
    cli_sock.Close();
    return 0;
}
