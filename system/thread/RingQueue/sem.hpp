#ifndef _SEM_HPP_
#define _SEM_HPP_

#include <iostream>
#include <semaphore.h>

class Sem
{
public:
    Sem(int value)
    {
        sem_init(&_sem, 0, value);  // 初始化
    }    

    void p()
    {
        sem_wait(&_sem);  // 申请信号量
    }

    void v()
    {
        sem_post(&_sem);  // 归还信号量
    }

    ~Sem()
    {
        sem_destroy(&_sem);  // 销毁
    }
    
private:
    sem_t _sem;

};

#endif