
#include "udp_server.hpp"
#include <memory>
#include <cstdlib>

static void usage1(std::string proc)
{
    std::cout << "\nUsage: " << proc << " port\n" << std::endl;
}

static void usage2(std::string proc)
{
    std::cout << "\nUsage: " << proc << " ip port\n" << std::endl;
}

void testAllClient(int* argc, char** argv[])    // 公网
{
    if (*argc != 2)
    {
        usage1((*argv)[0]);
        exit(1);
    }

    // std::string ip = argv[1];  服务器端不需要ip
   
    uint16_t port = atoi((*argv)[1]);   // 字符串转int
    std::unique_ptr<UdpServer> svr(new UdpServer(port));

    svr->initServer();
    svr->Start();
}

void testLocalClient(int* argc, char** argv[])   // 本地测试  127.0.0.1 8080
{
    if (*argc != 3)
    {
        usage2((*argv)[0]);
        exit(1);
    }

    std::string ip = (*argv)[1]; 
    uint16_t port = atoi((*argv)[2]);   // 字符串转int
   
    std::unique_ptr<UdpServer> svr(new UdpServer(port));

    svr->initServer();
    svr->Start();
    
}

// ./udp_server ip port   // 服务器端不需要ip
int main(int argc, char* argv[])
{   
    //testAllClient(&argc, &argv);    // 公网

    testLocalClient(&argc, &argv);   // 本地测试  127.0.0.1 8080

    return 0;
}