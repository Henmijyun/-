#include "threadPool.hpp"
#include "Task.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>


int main()
{
    // logMessage(NORMAL, "%s %d %c %f\n", "这是一条日志", 123, 'c', 3.14);

    srand((unsigned long)time(nullptr) ^ getpid());  // 随机值种子

    ThreadPool<Task>* tp = new ThreadPool<Task>();
    tp->run();

    while (true)
    {
        // 生产的过程，制作任务的时候，耗费时间
        int x = rand() % 100 + 1;
        usleep(7234);
        int y = rand() % 50 + 1;
        Task t(x, y, [](int x, int y)->int{
            return x + y;
        });    // lambda表达式

        //std::cout << "制作任务完成: " << x << "+" << y << "=?" << std::endl;
        logMessage(DEBUG, "制作任务完成: %d+%d=?", x, y);

        // 推送任务到线程池中
        tp->pushTask(t);

        sleep(1);
    }

    return 0;
}