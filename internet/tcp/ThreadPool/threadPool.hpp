#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unistd.h>
#include "lockGuard.hpp"
#include "thread.hpp"
#include "log.hpp"

const int g_thread_num = 10;


// 消费者模型
template<class T>
class ThreadPool
{
public:
    pthread_mutex_t* getMutex()
    {
        return &_lock;
    }

    bool isEmpty()
    {
        return _task_queue.empty();
    }

    void waitCond()
    {
        pthread_cond_wait(&_cond, &_lock);
    }

    T getTask()
    {
        T t = _task_queue.front();
        _task_queue.pop();
        return t;
    }

private:
    ThreadPool(int thread_num = g_thread_num)
        :_num(thread_num)
    {
        pthread_mutex_init(&_lock, nullptr);
        pthread_cond_init(&_cond, nullptr);
        
        for (int i = 1; i <= _num; i++)
        {
            // 创建线程
            _threads.push_back(new Thread(i, routine, this));
        }
    }

    ThreadPool(const ThreadPool<T>& other) = delete;
    ThreadPool<T>& operator=(const ThreadPool<T>& other) = delete;

public:
    // 多线程使用单例的过程
    static ThreadPool<T> *getThreadPool(int num = g_thread_num)
    {
        // 有效减少加锁检测的问题
        // 拦截大量的在已经创建好单例的时候，剩余线程请求单例的而直接访问锁的行为
        if (nullptr == _thread_ptr) 
        {
            lockGuard lockguard(&_mutex);
            // 任何一个线程想获取单例，都必须调用getThreadPool接口
            // 但是，大量的申请和释放锁的行为，是无用且浪费资源的
            // pthread_mutex_lock(&_mutex);
            if (nullptr == _thread_ptr)
            {
                _thread_ptr = new ThreadPool<T>(num);
            }
            // pthread_mutex_unlock(&_mutex);
        }
        return _thread_ptr;
    }

    void run()
    {
        for (auto &iter : _threads)
        {
            iter->start();
            //std::cout << iter->name() << " 启动成功" << std::endl;
            logMessage(NORMAL, "%s %s", iter->name().c_str(), "启动成功");
        }
    }

    // 线程池本质也是一个生产消费模型
    // 执行任务 (消费)
    static void* routine(void* args)   // static 不存在this指针，并且只能使用类内的静态方法
    {
        ThreadData* td = (ThreadData*)args;
        ThreadPool<T>* tp = (ThreadPool<T>*)td->_args;
        
        while (true)
        {
            T task;
            {
                lockGuard lockhguard(tp->getMutex());  // 创建对象自动加锁
                while (tp->isEmpty())
                {
                    tp->waitCond();  // 队列中没有任务就等待
                }
                // 读取任务
                task = tp->getTask();  // 任务队列是共享的->把任务从共享，拿到私有空间
            }
            // 处理任务
            task(td->_name);

        }
    }

    // 插入任务 (生产)
    void pushTask(const T& task) 
    {
        lockGuard lockhguard(&_lock);  // 创建对象自动加锁
        _task_queue.push(task);
        pthread_cond_signal(&_cond);   // 唤醒线程逐个执行
    }

    // test func
    void joins()
    {
        for (auto &iter : _threads)
        {
            iter->join();  // 等待
        }
    }

    void show()
    {
        std::cout << "可以传入this,让使用静态方法的线程，访问线程池内的方法"  << std::endl;
    }
    ~ThreadPool()
    {
        for (auto &iter : _threads)
        {
            //iter->join();  // 主线程先回收资源
            delete iter;   // 再释放
        }

        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_cond);
    }
    
private:
    std::vector<Thread*> _threads;   // 储存线程
    int _num;                       // 线程数
    std::queue<T> _task_queue;     // 任务队列
    pthread_mutex_t _lock;         // 锁 
    pthread_cond_t _cond;          // 条件变量

    static ThreadPool<T>* _thread_ptr;  // 线程池的指针
    static pthread_mutex_t _mutex;      // 单例模式的锁
    
    // 优化方案：
    //  queue1,queue2
    //  std::queue<T> *p_queue, *c_queue
    //  p_queue->queue1
    //  c_queue->queue2
    //  p_queue -> 生产一批任务之后，swap(p_queue,c_queue),唤醒所有线程/一个线程
    //  当消费者处理完毕的时候，你也可以进行swap(p_queue,c_queue)
    //  因为我们生产和消费用的是不同的队列，未来我们要进行资源的处理的时候，仅仅是指针

};

template <typename T>
ThreadPool<T> *ThreadPool<T>::_thread_ptr = nullptr;

template <typename T>
pthread_mutex_t ThreadPool<T>::_mutex = PTHREAD_MUTEX_INITIALIZER;