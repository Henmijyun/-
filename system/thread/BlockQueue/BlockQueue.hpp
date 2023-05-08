#pragma once 

#include <iostream>
#include <queue>
#include <mutex>
#include <pthread.h>
#include "lockGuard.hpp"


//#define INI_MTX(mtx) pthread_mutex_init(&mtx, nullptr)
//#define INI_COND(cond) pthread_cond_init(&cond, nullptr)

const int gDefaultCap = 5;

template<class T>
class BlockQueue
{
private:
    bool isQueueEmpty()  // 判空
    {
        return _bq.size() == 0;
    }

    bool isQueueFull()  // 判满
    {
        return _bq.size() == _capacity;
    }

public:
    BlockQueue(int capacity = gDefaultCap)
        :_capacity(capacity)
    {
        //INI_MTX(_mtx);
        pthread_mutex_init(&_mtx, nullptr);     // 创建锁
        pthread_cond_init(&_Empty, nullptr);    // 创建条件变量
        pthread_cond_init(&_Full, nullptr);     // 创建条件变量
    }

    void push(const T& in) // 生产者
    {
        // pthread_mutex_lock(&_mtx);
        // //1. 先检测当前的临界资源是否能够满足访问条件
        // // pthread_cond_wait: 第二个参数是一个锁，当成功调用wait之后，传入的锁，会被自动释放！
        // // 当被唤醒的时候，pthread_cond_wait，会自动帮助我们线程获取锁
        // // pthread_cond_wait: 但是只要是一个函数，就可能调用失败
        // // pthread_cond_wait: 可能存在 伪唤醒 的情况
        // while(isQueueFull()) pthread_cond_wait(&_Full, &_mtx); // 循环检测
        // //2. 访问临界资源，100%确定，资源是就绪的！
        // _bq.push(in);
        // if(_bq.size() >= _capacity/2) 
        // {
        //     pthread_cond_signal(&_Empty);
        // }
        // pthread_cond_signal(&_Empty);
        // pthread_mutex_unlock(&_mtx);

        lockGuard lockgrard(&_mtx); // 自动调用构造函数

        while (isQueueFull())
        {
            pthread_cond_wait(&_Full, &_mtx);  
        }
        // 2. 访问临界资源，100%确定，资源是就绪的！
        _bq.push(in);
        pthread_cond_signal(&_Empty);  
    }// 自动调用lockgrard 析构函数

    void pop(T* out) // 消费者
    {
        lockGuard lockguard(&_mtx);
        // pthread_mutex_lock(&_mtx);
        
        while (isQueueEmpty())
        {
            pthread_cond_wait(&_Empty, &_mtx);
        }    
        *out = _bq.front();
        _bq.pop();

        pthread_cond_signal(&_Full);

        // pthread_mutex_unlock(&_mtx);
    }
    
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_Empty);
        pthread_cond_destroy(&_Full);
    }

private:
    std::queue<T> _bq;      // 阻塞队列
    int _capacity;          // 容量上限
    pthread_mutex_t _mtx;   // 通过互斥锁保证队列安全
    pthread_cond_t _Empty; // 用来表示bq 是否空的条件
    pthread_cond_t _Full;  // 用来表示bq 是否满的条件
};