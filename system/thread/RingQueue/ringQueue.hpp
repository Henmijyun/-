#ifndef _RING_QUEUE_HPP_
#define _RING_QUEUE_HPP_

#include <iostream>
#include <vector>
#include <pthread.h>
#include "sem.hpp"

// 多生产多消费的主要意义不在于数据的交换
// 而是数据或者任务在 交换前生产 和 交换后消费 的处理，才是最耗费时间的。
// 生产的本质：私有的任务-> 公共空间中
// 消费的本质：公共空间中的任务-> 私有的

// 信号量本质是一把计数器-> 不用进入临界区，就可以得知资源情况，减少临界区内部的判断！
// 申请锁 -> 判断与访问 -> 释放锁 --> 本质是我们并不清楚临界资源的情况！！
// 信号量要提前预设资源的情况，而且在pv变化过程中，我们可以在外部就能知晓临界资源的情况！


const int g_default_num = 5;

// 环形队列-消费者模型
// 多线程(多消费者+多生产者)
template<class T>
class RingQueue
{
public:
    RingQueue(int default_num = g_default_num)
        :_ring_queue(default_num)
        ,_num(default_num)
        ,_c_step(0)
        ,_p_step(0)
        ,_space_sem(default_num)
        ,_data_sem(0)
    {
        pthread_mutex_init(&_plock, nullptr);  // 初始化锁
        pthread_mutex_init(&_clock, nullptr);
    }

    ~RingQueue()
    {
        pthread_mutex_destroy(&_plock);   // 销毁锁
        pthread_mutex_destroy(&_clock);
    }

    // 生产者:空间资源
    void push(const T& in)
    {
        _space_sem.p();  // 申请空间信号量

        pthread_mutex_lock(&_plock);   // 先拿到信号，再竞争锁，只有一个线程可以进入临界区
        _ring_queue[_p_step++] = in;
        _p_step %= _num;
        pthread_mutex_unlock(&_plock);

        _data_sem.v();  // 归还数据信号量
    }

    // 消费者：数据资源
    void pop(T* out)
    {
        _data_sem.p();  // 申请数据信号量
        
        pthread_mutex_lock(&_clock);   // 先拿到信号，再竞争锁，只有一个线程可以进入临界区
        *out = _ring_queue[_c_step++];
        _c_step %= _num;
        pthread_mutex_unlock(&_clock);

        _space_sem.v();  // 归还空间信号量
    }


    void debug()
    {
        std::cerr << "size: " << _ring_queue.size() 
        << " num: " << _num << std::endl;
    }

private:
    std::vector<T> _ring_queue;  // 容器
    int _num;        // 环形队列的大小
    int _p_step;     // 生产下标
    int _c_step;     // 消费下标 
    Sem _space_sem;  // 空间信号
    Sem _data_sem;   // 数据信号
    pthread_mutex_t _plock;  // 生产锁 (生产者内部竞争)
    pthread_mutex_t _clock;   // 消费锁 (消费者内部竞争)
};

#endif