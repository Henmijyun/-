
#include <iostream>
#include "Sock.hpp"
#include "Protocol.hpp"

using namespace skk_protocol;

static void Usage(const std::string& process)
{
    std::cout << "\nUsage: " << process << " server_ip server_port\n" << std::endl; 
}

// ./client server_ip server_port
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    Sock sock;
    int sockfd = sock.Socket();  // 创建套接字
    if (!sock.Connect(sockfd, server_ip, server_port))  // 连接 成功true
    {
        std::cerr << "Connect error" << std::endl;
        exit(2);
    }
    
    while (true)
    {
        Request req(10, 20, '*');
        std::string s = req.Serialize();  // 序列化
        //std::cout << "s:" << s << std::endl;
        Send(sockfd, s);  // 发送

        std::string r = Recv(sockfd);  // 读
        //std::cout << "r:" << r << std::endl;

        Response resp;
        resp.Deserialized(r);   // 反序列化
        
        std::cout << "code: " << resp._code << std::endl;
        std::cout << "result: " << resp._result << std::endl;
        
        sleep(1);
    }

    return 0;
}