
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
    
    bool quit = false;
    std::string buffer;
    
    while (!quit)
    {
        // Request req(10, 20, '*');
        // 1.获取需求
        Request req;
        std::cout << "Please Enter # ";
        std::cin >> req._x >> req._op >> req._y;

        // 2.序列化
        std::string s = req.Serialize();  // 序列化
        // std::string temp = s;
        
        // 3.添加长度报头
        s = Encode(s);

        // 4.发送给服务端
        Send(sockfd, s);  // 发送

        // 5.正常读取
        while (true)
        {
            bool res = Recv(sockfd, &buffer);  // 读
            if (!res)
            {
                // 读取失败
                quit = true;
                break;
            }
            std::string package = Decode(buffer);  // 解析/删除 报头
            if (package.empty())
            {
                // 空(不完整报文)
                continue;
            }
            
            Response resp;
            resp.Deserialized(package);   // 反序列化
            std::string err;
            switch(resp._code)
            {
            case 1:
                err = "除0错误"; 
                break;
            case 2:
                err = "模0错误"; 
                break;
            case 3:
                err = "非法操作"; 
                break;
            default:
                std::cout << resp._x << (char)resp._op << resp._y << " = " << resp._result << " [success]" << std::endl;                
                break;            
            }

            if (!err.empty())
            {
                std::cerr << err << std::endl;
            }
            // sleep(1);
            break;
        }

    }
    
    close(sockfd);

    return 0;
}