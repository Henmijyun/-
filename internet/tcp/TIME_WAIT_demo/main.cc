
#include "Sock.hpp"

int main()
{
    Sock sock;
    int listensock = sock.Socket();   // 创建套接字
    sock.Bind(listensock, 8080);  // 绑定
 
    sock.Listen(listensock);  // 监听
    
    while (true)
    {
        std::string client_ip;
        uint16_t client_port;
        int sockfd = sock.Accept(listensock, &client_ip, &client_port);  // 获取文件描述符
        if (sockfd > 0)
        {
            // 获取成功
            std::cout << "[" << client_ip << ":" << client_port << "]# " << sockfd << std::endl; 
        }

        // 服务端CLOSE_WAIT状态

        sleep(10);

        close(sockfd);
        std::cout << sockfd << " closed" << std::endl;
        // 服务端TIME_WAIT状态
    }
    return 0;
}