// 单线程版本

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " serverIp serverPort\n" << std::endl;
}

// ./udp_client 127.0.0.1 8080 
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }

    // client 也需要bind绑定,但是一般client不会显示的bind,由OS随机bind.
    std::string message;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));     // 端口号 -> 字符串 -> 整数 -> 网络
    server.sin_addr.s_addr = inet_addr(argv[1]);   // 字符串 转 IP序列

    char buffer[1024];
    while (true)
    {
        std::cout << "请输入你的信息# ";
        std::getline(std::cin, message);   // 从流中读取到message
        if (message == "quit")
        {
            break;
        }

        // 当cilent首次发送消息给服务器的时候,OS会自动给client bind他的IP和PORT
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));   // 把数据发送给服务器

        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        ssize_t s = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&temp, &len);  // 虽然是从客户端发出去,但是UPD接口也保留了接收功能
        if (s > 0)
        {
            buffer[s] = 0;
            std::cout << "server echo# " << buffer << std::endl;
        }
    }

    close(sock);

    return 0;
}