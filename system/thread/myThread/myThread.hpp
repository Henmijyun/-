#pragma once

#include <iostream>
#include <thread>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <cassert>
#include <cstdio>

using namespace std;

#define THREAD_NUM 10

int tickets = 10000; // 在并发访问时，导致数据不一致的问题！临界资源
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 全局/静态锁 的定义 
// pthread_mutex_t 就是原生线程库提供的一个数据类型
// 加锁保护：加锁的时候，一定要保证加锁的粒度，越小越好！！

class ThreadData
{
public:
    ThreadData(const string& n, pthread_mutex_t* pm)
        :tname(n)
        ,pmtx(pm)
    {}   

public:
    string tname;          // 线程名
    pthread_mutex_t* pmtx; // 锁
};

void* getTickets(void* args)
{
    // int myerrno = errno;  // 备份

    ThreadData* td = (ThreadData*)args;
    while (true)
    {
        // 抢票逻辑
        int n = pthread_mutex_lock(td->pmtx);  // 上锁 成功返回0
        assert(n == 0);

        // 临界区
        if (tickets > 0)  // 1. 判断的本质也是计算的一种
        {
            usleep(rand() % 1500);
            printf("%s: %d\n", td->tname.c_str(), tickets);
            tickets--;  // 2. 也可能出现问题
            
            n = pthread_mutex_unlock(td->pmtx);  // 解锁
            assert(n == 0);         
        } 
        else
        {
            n = pthread_mutex_unlock(td->pmtx);  // 解锁
            assert(n == 0);            
            break;
        }

        // 抢完票，其实还需要后续的动作
        usleep(rand() % 2000);
        
        // errno = myerrno;  // 恢复备份  这种方法也可以实现可重入性
    } 
    delete td;
    return nullptr;
}

void testMutex()
{
    time_t start = time(nullptr);  // 生成起始时间

    pthread_mutex_t mtx;   // 创建 锁
    pthread_mutex_init(&mtx, nullptr);  // 初始化锁

    srand((unsigned long)time(nullptr) ^ getpid() ^ 0x123); // 让随机数更随机

    pthread_t t[THREAD_NUM];  // 线程
    // 多线程抢票逻辑
    for (int i = 0; i < THREAD_NUM; ++i)
    {
        string name = "thread ";
        name += to_string(i);  // 10转string，再+=
        ThreadData* td = new ThreadData(name, &mtx);   // 创建描述线程的类
        pthread_create(t + i, nullptr, getTickets, (void*)td);  // 创建线程
    }

    // 线程等待
    for (int i = 0; i < THREAD_NUM; ++i)
    {
        pthread_join(t[i], nullptr); 
    }

    pthread_mutex_destroy(&mtx);  // 销毁锁

    time_t end = time(nullptr);  // 结束时间

    cout << "cast: " << (int)(end - start) << "S" << endl;
}
