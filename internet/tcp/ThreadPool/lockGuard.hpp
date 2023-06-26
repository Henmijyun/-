#include <iostream>
#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t* mtx)
        :_pmtx(mtx)
    {}

    void lock()
    {
        //std::cout << "要进行加锁" << std::endl;
        pthread_mutex_lock(_pmtx);
    }

    void unlock()
    {
        //std::cout << "要进行解锁" << std::endl;
        pthread_mutex_unlock(_pmtx);
    }

    ~Mutex()
    {}

private:
    pthread_mutex_t* _pmtx;

};

// RAII风格的加锁方式
class lockGuard
{
public:
    lockGuard(pthread_mutex_t* mtx)
        :_mtx(mtx)
    {
        _mtx.lock();
    }

    ~lockGuard()
    {
        _mtx.unlock();
    }

private:
    Mutex _mtx;
};