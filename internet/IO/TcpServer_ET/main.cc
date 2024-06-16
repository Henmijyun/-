#include "TcpServer.hpp"

#include <memory>

int main()
{
    std::unique_ptr<TcpServer> svr(new TcpServer());
    svr->Dispather(); // 任务派发
    return 0;
}