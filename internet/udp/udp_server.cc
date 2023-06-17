
#include "udp_server.hpp"
#include <memory>
#include <cstdlib>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " port\n" << std::endl;
}

// ./udp_server ip port   // 服务器端不需要ip
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(1);
    }

    // std::string ip = argv[1];  服务器端不需要ip
    // uint16_t port = atoi(argv[2]);   // 字符串转int
   
    uint16_t port = atoi(argv[1]);   // 字符串转int
    std::unique_ptr<UdpServer> svr(new UdpServer(port));

    svr->initServer();
    svr->Start();
    
    return 0;
}