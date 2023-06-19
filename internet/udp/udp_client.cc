// 多线程版本

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory>
#include "thread.hpp"

// 1.无论是多线程读还是写，sock都是只有一个，sock代表就是文件，UDP是全双工的->可以同时进行收发而不受干扰

uint16_t serverport = 0;
std::string serverip;

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " serverIp serverPort\n"
              << std::endl;
}

static void* udpSend(void* args) // 负责发送数据的线程
{
    int sock = *(int*)((ThreadData*)args)->_args;
    std::string name = ((ThreadData*)args)->_name; // 提取名字和套接字

    std::string message;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);                  // 端口号 -> 字符串 -> 整数 -> 网络
    server.sin_addr.s_addr = inet_addr(serverip.c_str()); // 字符串 转 IP序列

    while (true)
    {
        std::cerr << "请输入你的信息# ";  // 标准错误 2文件描述符
        std::getline(std::cin, message); // 从流中读取到message
        if (message == "quit")
        {
            break;
        }

        // 当cilent首次发送消息给服务器的时候,OS会自动给client bind他的IP和PORT
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server)); // 把数据发送给服务器
    }

    return nullptr;
}

static void* udpRecv(void* args) // 负责接收数据的线程
{
    int sock = *(int*)((ThreadData*)args)->_args;
    std::string name = ((ThreadData*)args)->_name; // 提取名字和套接字

    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));    // 清空buffer
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        ssize_t s = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&temp, &len); // 虽然是从客户端发出去,但是UPD接口也保留了接收功能
        if (s > 0)
        {
            buffer[s] = 0;
            std::cout << buffer << std::endl;
        }
    }
}

// ./udp_client 127.0.0.1 8080
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);   // 创建套接字
    if (sock < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }

    serverport = atoi(argv[2]);
    serverip = argv[1];

    // 智能指针控制线程
    std::unique_ptr<Thread> sender(new Thread(1, udpSend, (void*)&sock));
    std::unique_ptr<Thread> recver(new Thread(2, udpRecv, (void*)&sock));

    sender->start();
    recver->start();

    sender->join();
    recver->join();

    close(sock);

    return 0;
}