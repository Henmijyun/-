// 大小写转换版本的tcp_cilent

#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>



void usage(std::string proc)
{
    std::cout << "Usage: " << proc << " serverIp serverPort\n" << std::endl; 
}

// ./tcp_client targetIp targetPort
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }
    
    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);
    bool alive = false;
    int sock = 0;

    while (true)
    {
        
        
        if (!alive)
        {
            //std::cout << "000000000" << std::endl;
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0)
            {
                std::cerr << "sock error" << std::endl;
                exit(2);
            }

            // client 不需要显示bing，系统自动port选择 
            // 转网络序列
            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons(server_port);
            server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    

            // 连接服务器
            if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
            {
                std::cerr << "connect error" << std::endl;
                exit (3);
            }
            std::cout << "connect success" << std::endl;
            
            alive = true;
        }

        // 输入
        std::string line;
        std::cout << "请输入# ";
        std::getline(std::cin, line);
        if (line == "quit")
        {
            break;
        }

        ssize_t s1 = send(sock, line.c_str(), line.size(), 0);   // 写到服务器
        if (s1 > 0)
        {
            char buffer[1024];
            size_t s2 = recv(sock, buffer, sizeof(buffer)-1, 0);   // 从服务器读
            if (s2 > 0)
            {
                //std::cout << "11111111" << std::endl;
                buffer[s2] = 0;
                std::cout << "server 回显# " << buffer << std::endl;
            }
            else if(s2 == 0)
            {
                //td::cout << "222222222" << std::endl << std::endl;
                alive = false;
                close(sock);
            }
        }
        else
        {
            //std::cout << "333333333" << std::endl;
            alive = false;
            close(sock);
        }
    }
    
    return 0;
}