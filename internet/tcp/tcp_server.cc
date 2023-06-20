
#include "tcp_server.hpp"
#include <memory>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " port\n" << std::endl;
}


// ./tcp_server port
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }
    
    uint16_t port = atoi(argv[1]);     // 字符串转整型
    std::unique_ptr<TcpServer> svr(new TcpServer(port));   // 智能指针管理svr

    svr->initServer();   // 初始化
    svr->Start();        // 运行

    return 0;
}