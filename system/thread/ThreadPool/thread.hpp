#include <iostream>
#include <string>
#include <functional>
#include <cstdio>

//typedef std::function<void* (void*)> fun_t;  // 仿函数
typedef void*(*fun_t)(void*);  // 函数指针

class ThreadData
{
public:
    void* _args;        // 回调方法的参数
    std::string _name;  // 线程名
};

class Thread
{
public:
    Thread(int num, fun_t callback, void* args)
        :_func(callback)
    {
        char nameBuffer[64];
        snprintf(nameBuffer, sizeof nameBuffer, "Thread-%d", num);
        _name = nameBuffer;

        _tdata._args = args;
        _tdata._name = _name;
    }

    void start()
    {
        pthread_create(&_tid, nullptr, _func, (void*)&_tdata);  // 创建线程
    }

    void join()
    {
        pthread_join(_tid, nullptr);   // 线程等待
    }

    std::string name()
    {
        return _name;
    }

    ~Thread()
    {}

private:
    std::string _name;  // 线程名
    fun_t _func;       // 回调方法
    ThreadData _tdata;  // 线程数据        
    pthread_t _tid;     // 线程ID
};
