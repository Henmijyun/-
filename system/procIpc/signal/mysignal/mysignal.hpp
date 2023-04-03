#pragma once

#include <iostream>
#include <cassert>
#include <unistd.h>
#include <signal.h>



void catchSig(int signum)
{
    std::cout << "获取一个信号: " << signum << std::endl;
}

// test_1  捕捉信号  （只有9杀死，19暂停，20暂停，这3种不能被获取）
void getSig()
{
    for(int sig = 1; sig <= 31; sig++) 
    {
        signal(sig, catchSig);
    }
    while(true) 
    {
        sleep(1);
    }
}


static void handler1(int signum) // static 只在该函数所在的文件作用域内可见
{
    std::cout << "捕捉 信号： " << signum << std::endl;
}

static void showPending(sigset_t& pending)
{
    for (int sig = 1; sig <= 31; ++sig)
    {
        if (sigismember(&pending, sig)) // 查询信号是否在信号集合之中
        {
            std::cout << "1";
        }
        else
        {
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

// test_2 捕捉信号 kill -2，观察20秒内，阻塞信号集的变化
void captureSig()
{
    // 0.为了方便测试，捕获2号信号，让进程2号时不要退出
    signal(2, handler1);

    // 1.定义信号集对象
    sigset_t bset, obset;
    sigset_t pending;

    // 2.初始化
    sigemptyset(&bset);
    sigemptyset(&obset);
    sigemptyset(&pending);

    // 3.添加要进行屏蔽的信号
    sigaddset(&bset, 2 /*SIGINT*/);

    // 4. 设置set到内核中对应的进程内部[默认情况进程不会对任何信号进行block]
    int n = sigprocmask(SIG_BLOCK, &bset, &obset);
    assert(n == 0);
    (void)n;

    std::cout << "block 2 号信号成功...., pid: " << getpid() << std::endl;

    // 5.重复打印当前进程的pending信号集
    int count = 0;
    while (true)
    {
        // 5.1 获取当前进程的pending信号集
        sigpending(&pending);

        // 5.2 显示pending信号集中的没有被递达的信号
        showPending(pending);
        sleep(1);
        count++;
        if (count == 20)
        {
            // 默认情况下，恢复对于2号信号的block的时候，确实会进行递达
            // 但是2号信号的默认处理动作是终止进程！
            // 需要对2号信号进行捕捉
            std::cout << "解除对于2号信号的block" << std::endl;
            int n = sigprocmask(SIG_SETMASK, &obset, nullptr);  // 恢复默认，之前保存的obset信号集
            assert(n == 0);
            (void)n;
        } 
    }

}


static void blockSig(int sig) // 添加到block阻塞信号集
{
    sigset_t bset;
    sigemptyset(&bset);   // 初始化信号集
    sigaddset(&bset, sig);  // sig加入到信号集之中
    int n = sigprocmask(SIG_BLOCK, &bset, nullptr);  // 增加bset信号集到当前进程的阻塞集之中
    assert(n == 0);
    (void)n;
}

// test_3 把1~31信号 全部添加到阻塞信号集block
// 观察是否能全部阻塞
void blockSig()
{
    for(int sig = 1; sig <= 31; sig++)
    {
        blockSig(sig); // 1~31 添加到阻塞信号集
    }
    sigset_t pending;
    while(true)
    {
        sigpending(&pending);   // 读取当前进程的未决信号集
        showPending(pending);   // 打印
        sleep(1);
    }
}
// 9结束 19暂停 20暂停 无法被添加到阻塞信号集




void showPending2(sigset_t *pending)
{
    for(int sig = 1; sig <= 31; sig++)
    {
        if(sigismember(pending, sig)) 
        {
            std::cout << "1";
        }
        else 
        {
            std::cout << "0";
        }    
    }
    std::cout << std::endl;
}

void handler2(int signum)
{
    std::cout << "获取了一个信号： " << signum << std::endl;
    std::cout << "获取了一个信号： " << signum << std::endl;
    std::cout << "获取了一个信号： " << signum << std::endl;
    std::cout << "获取了一个信号： " << signum << std::endl;

    sigset_t pending;
    int c = 20;
    while (true)
    {
        sigpending(&pending);
        showPending2(&pending);
        c--;
        if (!c)
        {
            break;
        }
        sleep(1);
    }
}

// test_4 在任何时刻，只处理一层信号
void sigProcOne()
{
    std::cout << "getpid: " << getpid() << std::endl;
    // 内核数据类型，用户栈定义的
    struct sigaction act, oact;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);  // 初始化
    act.sa_handler = handler2;

    sigaddset(&act.sa_mask, 3);  // 将信号signum 加入到信号集之中
    sigaddset(&act.sa_mask, 4);
    sigaddset(&act.sa_mask, 5);
    sigaddset(&act.sa_mask, 6);
    sigaddset(&act.sa_mask, 7);

    // 设置进当前调用进程的pcb中
    sigaction(2, &act, &oact);

    std::cout << "default action : " << (int)(oact.sa_handler) << std::endl;



    while(true) sleep(1);
}