// 高级IO:非阻塞功能

#include <iostream>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <cassert>

#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

// fcntl 给一个文件描述符新增非阻塞功能
bool SetNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL); // 在底层获取当前fd对应的文件读写标记位
    if (fl < 0)
    {
        return false; // 失败
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK); // 新增非阻塞的标记位
    return true;
}

// 测试fcntl
void testFcntl()
{
    // 标准输入 0
    // 非阻塞的时候，我们是以出错的形式返回，告知上层数据没有就绪
    // a.我们如何甄别是真的出错了
    // b.还是仅仅是数据，没有就绪？
    // 数据就绪了的话，我们就正常读取就行。
    SetNonBlock(0); // 只要设置一次，后续就一直是非阻塞了。
    char buffer[1024];
    while (true)
    {
        sleep(1);

        errno = 0;
        ssize_t s = read(0, buffer, sizeof(buffer) - 1); // 出错，不仅仅是错误返回值，errno变量也会被设置，表明出错原因
        if (s > 0)
        {
            // 读取成功
            buffer[s - 1] = 0;
            std::cout << "echo# " << buffer << " errno[---]: " << errno << " errstring: " << strerror(errno) << std::endl;
        }
        else
        {
            // 如果失败的errno值是11，就代表其实没错，只不过底层数据没就绪。
            // std::cout << "read \"errer\"" <<  " errno: " << errno << " errstring: " << strerror(errno) <<std::endl;

            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                std::cout << "当前0号fd数据没有就绪，再试试 " << std::endl;
                continue;
            }
            else if (errno == EINTR)
            {
                std::cout << "当前IO可能被信号中断，再试试 " << std::endl;
                continue;
            }
            else
            {
                // 进行差错处理
            }
        }
    }
}

// 测试select
void testSelect()
{
    while (true)
    {
        // 获取时间
        std::cout << "time: " << (unsigned long)time(nullptr) << std::endl;
        struct timeval currtime = {0, 0};
        int n = gettimeofday(&currtime, nullptr);
        assert(n == 0);
        (void)n;

        std::cout << "gettimeofday: " << currtime.tv_sec << "." << currtime.tv_usec << std::endl;
        sleep(1);
    }
}

int main()
{
    // testFcntl();  // 测试fcntl
    testSelect(); // 测试select
    return 0;
}