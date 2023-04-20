#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

using namespace std;

#define TNUM 4

typedef void (*func_t)(const string& name, pthread_mutex_t* pmtx, pthread_cond_t* pcond);
volatile bool quit = false;

// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // 锁 全局或静态
// pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


class ThreadData
{
public:
    ThreadData(const string& name, func_t func, pthread_mutex_t* pmtx, pthread_cond_t* pcond)
        :name_(name)
        ,func_(func)
        ,pmtx_(pmtx)
        ,pcond_(pcond)
    {}

public:
    string name_;   // 名字 
    func_t func_;   // 方案
    pthread_mutex_t* pmtx_;  // 锁
    pthread_cond_t* pcond_;  // 条件变量
};


void func1(const string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while(!quit)
    {
        // wait一定要在加锁和解锁之间进行wait！
        // v2: 
        pthread_mutex_lock(pmtx);
        // if(临界资源是否就绪-- 否) pthread_cond_wait
        pthread_cond_wait(pcond, pmtx); //默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        cout << name << " running -- 播放" << endl;
        pthread_mutex_unlock(pmtx);
    }
}

void func2(const string &name,pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while(!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); //默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        cout << name << " running  -- 下载" << endl;
        pthread_mutex_unlock(pmtx);

    }
}
void func3(const string &name,pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while(!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); //默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        cout << name << " running  -- 刷新" << endl;
        pthread_mutex_unlock(pmtx);

    }
}
void func4(const string &name,pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while(!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); //默认该线程在执行的时候，wait代码被执行，当前线程会被立即被阻塞
        cout << name << " running  -- 扫码用户信息" << endl;
        pthread_mutex_unlock(pmtx);
    }
}

void* Entry(void* args)
{
    ThreadData* td = (ThreadData*)args;   // td在每一个线程自己私有的栈空间中保存
    td->func_(td->name_, td->pmtx_, td->pcond_);  // 它是一个函数，调用完成就要返回！
    delete td;
    return nullptr;
}

int main()
{
    pthread_mutex_t mtx;  // 锁
    pthread_cond_t cond;  // 条件变量   
    pthread_mutex_init(&mtx, nullptr);  // 初始化
    pthread_cond_init(&cond, nullptr);

    pthread_t tids[TNUM];
    func_t funcs[TNUM] = {func1, func2, func3, func4};
    for (int i = 0; i < TNUM; i++)
    {
        string name = "Thread ";
        name += to_string(i + 1);
        ThreadData* td = new ThreadData(name, funcs[i], &mtx, &cond);
        pthread_create(tids + i, nullptr, Entry, (void*)td);  // 创建线程 
    }

    sleep(5);

    // ctrl new thread
    int cnt = 10;
    while (cnt)
    {
        cout << "resume thread run code ...." << cnt-- << endl;

        pthread_cond_signal(&cond);      // 线程排队顺序运行
        // pthread_cond_broadcast(&cond);   // 全部线程一起运行

        sleep(1);
    }
    
    cout << "ctrl done" << endl;
    quit = true;
    pthread_cond_broadcast(&cond);   // 全部线程一起运行 (为了让线程们退出while)

    for(int i = 0; i < TNUM; i++)
    {
        pthread_join(tids[i], nullptr);   // 线程等待
        cout << "thread: " << tids[i] << "   quit" << endl;
    }

    pthread_mutex_destroy(&mtx);   // 销毁锁
    pthread_cond_destroy(&cond);   // 销毁条件变量

    return 0;
}