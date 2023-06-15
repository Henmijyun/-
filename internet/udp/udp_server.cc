
#include "udp_server.hpp"
#include <memory>
#include <cstdlib>

static void usage(std::string proc)
{
    std::cout << "\nUsage: " << proc << " ip port\n" << std::endl;
}

// ./udp_server ip port 
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);   // 字符串转int
    std::unique_ptr<UdpServer> svr(new UdpServer(port, ip));

    svr->initServer();
    svr->Start();
    
    return 0;
}