#include <iostream>
#include <vector>
#include <functional>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

typedef std::function<void ()> func;
std::vector<func> callbacks;

uint64_t count = 0;

void showCount()
{
    std::cout << "final count : " << count << std::endl;
}

void showLog()
{
    std::cout << "这个是日志功能" << std::endl;
}

void logUser()
{
    if (fork() == 0)
    {
        // child
        execl("/usr/bin/who", "who", nullptr);  // 进程替换
        exit(1);
    }
    wait(nullptr);  // 阻塞，接收子进程
}

void flushData()
{
    std::cout << "这个是刷新缓冲区功能" << std::endl;
}

// 定时器功能
void catchSig(int signum)
{
    for(auto &f : callbacks)
    {
        f();  // 调用
    }
    std::cout << "signum: " << signum << std::endl << std::endl;

    alarm(5);  // 重新设置计时器，前面的计时器过期就永久作废。
}


int main()
{
    // 验证1s之内，我们一共会进行多少次count++
    // 1. 为什么我们只计算到1w+左右呢？cout打印 + 网络发送 = IO
    // 2. 如果单纯向计算算力:
    signal(SIGALRM, catchSig);    // SIGALRM -> 14  闹钟的信号 alarm()

    alarm(1); // 设定了一个闹钟，这个闹钟一旦触发，就自动移除了


    callbacks.push_back(showCount);
    callbacks.push_back(showLog);
    callbacks.push_back(logUser);
    callbacks.push_back(flushData);

    while (true)
    {
        count++;
        // 进程执行途中，闹钟时间到了，中断去完成定时任务
    }
    return 0;
}
