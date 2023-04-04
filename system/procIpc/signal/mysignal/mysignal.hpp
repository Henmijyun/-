#pragma once

#include <iostream>
#include <cassert>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
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

//*******************************************

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


//*******************************************

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



//*******************************************
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






//*******************************************

volatile int flag = 0;  // volatile关键字 
// 让指定的变量，每次都要去访问内存

void changeFlag(int signum)
{
    (void)signum;
    std::cout <<"change flag: "<< flag;
    flag = 1;
    std::cout << "->" << flag << std::endl;
}

// test_5 编译器优化导致的信号无法接收问题
void testVolatile()
{
    signal(2, changeFlag);
    while(!flag);
    std::cout << "进程正常退出后：" << flag << std::endl;
}


//*******************************************
void handler3(int signum)
{
    pid_t id;
    while ((id = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        std::cout << "wait child success: " << id << std::endl;
    }
    std::cout << "child is quit! " << getpid() << std::endl;
}

void testSigchld()
{
    signal(SIGCHLD, handler3);
    pid_t cid;
    if ((cid = fork()) == 0)
    {
        // child
        std::cout << "child: " << getpid() << std::endl;
        sleep(3);
        exit(1);
    }

    while (1)
    {
        std::cout << "father proc is doing some thing! pid: " << getpid() << std::endl;
        sleep(1);
    }
}

/*
// // 如果我们不想等待子进程，并且我们还想让子进程退出之后，自动释放僵尸子进程
// int main()
// {
//     // OS 默认就是忽略的
//     signal(SIGCHLD, SIG_IGN); // 手动设置对子进程进行忽略

//     if(fork() == 0)
//     {
//         cout << "child: " << getpid() << endl;
//         sleep(5);
//         exit(0);
//     }

//     while(true)
//     {
//         cout << "parent: " << getpid() << " 执行我自己的任务!" << endl;
//         sleep(1);
//     }
// }


// vector<int> pids;
void handler(int signum)
{
    cout << "子进程退出: " << signum << "fater: " << getpid() << endl;
    // wait();
    //10 子进程都退出
    // while(wait())
    // 10 5个子进程退出？
}

// 证明 子进程退出，会想父进程发送信号
int main()
{
    signal(SIGCHLD, handler);
    if(fork() == 0)
    {
        cout << "child pid: " << getpid() << endl;
        sleep(100);
        exit(0);
    }

    while(true) sleep(1);

}

*/