//高级IO poll多路转接

#include "pollServer.hpp"
#include <memory>


#include <poll.h>
#include <unistd.h>
#include <stdio.h>

// 监控输入流
void testTimeIn()
{
    struct pollfd poll_fd;
    poll_fd.fd = 0;
    poll_fd.events = POLLIN;
    for (;;)
    {
        int ret = poll(&poll_fd, 1, 1000);
        if (ret < 0)
        {
            perror("poll");
            continue;
        }
        if (ret == 0)
        {
            printf("poll timeout\n");
            continue;
        }
        if (poll_fd.revents == POLLIN)
        {
            printf("poll event read!\n");
            char buf[1024] = {0};
            read(0, buf, sizeof(buf) - 1);
            printf("stdin:%s", buf);
        }
    }
}

int main()
{
    std::unique_ptr<PollServer> svr(new PollServer());
    svr->Start();
    
    return 0;
}