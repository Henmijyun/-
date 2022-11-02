#include <iostream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef void (*handler_t)(); // 函数指针类型
std::vector<handler_t> handlers; // 函数指针数组

void func_one()
{
    std::cout << "这是一个临时任务1" << std::endl;
}

void func_two()
{
    std::cout << "这时一个临时文件2" << std::endl;
}

// 设置对应的方法回调
// 以后想让父进程闲了执行任何方法的时候，只要向Load里面注册，就可以让父进程执行对应的方法

void Load()
{
    handlers.push_back(func_one);
    handlers.push_back(func_two);
    // ..
}

int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        // 子进程
        int cnt = 3;
        while (cnt--)
        {
            std::cout << "我是子进程: " << cnt;
            std::cout << std::endl;

            sleep(1);
        }
        exit(5); // 5 用于测试
    }
    else
    {
        int quit = 0;
        while (!quit)
        {
            int status = 0;
            pid_t res = waitpid(-1, &status, WNOHANG); // 以非阻塞方式等待
            if (res > 0)
            {
                // 等待成功 && 子进程退出
                std::cout << "等待子进程退出成功，退出码是：" << WEXITSTATUS(status);
                std::cout << std::endl;
                quit = 1;
            }
            else if (res == 0)
            {
                // 等待成功 && 但子进程并未退出
                std::cout << "子进程还在运行，还没退出，父进程可以先干其他事" << std::endl;
                if (handlers.empty())
                    Load(); // 为空时，插入任务
                for (auto iter : handlers)
                {
                    // 父进程遍历执行处理其他任务
                    iter();
                }
            }
            else 
            {
                // 等待失败
                std::cout << "wait失败!" << std::endl;
                quit = 1;
            }
            sleep(1);
        }
    }
}

