#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <pthread.h>

using namespace std;

// 1. 线程谁先运行与调度器相关
// 2. 线程一旦异常，都可能导致整个进程整体退出
// 3. 线程的输入和返回值问题
// 4. 线程异常退出的理解


int x = 100;

__thread int num = 10;  // __thread 让每个线程各自拥有一个全局变量--线程的局部存储

void show(const string& name)
{
   cout << name << ", pid: " << getpid() << " " << x << endl <<  endl;
}

void* threadRun(void* args)
{
    const string name = (char*)args;
    while (true)
    {
        show(name);
        sleep(1);
    }
}

void testThread1()  // 创建多线程
{
    pthread_t tid[5];  // 线程的数据类型unsigned long int
    char name[64];

    for (int i = 0; i < 5; ++i)
    {
        snprintf(name, sizeof(name), "%s-%d", "thread", i);  // 读到数组中
        pthread_create(tid + i, nullptr, threadRun, (void*)name);  // 调用线程函数
        sleep(1); // 缓解传参的bug
    }

    while (true)
    {
        cout << "main thread, pid: " << getpid() << endl;
        sleep(3);
    }

}

/////////////////////////////////////////////////

void* threadRoutine1(void* args)
{
    int i = 0;
    while (true)
    {
        cout << "新线程： " << (char *)args << " running ..." << endl;
        sleep(1);
        if (i++ == 10)
        {
            break;
        }
        int a  = 100;
        a /= 0;  // 除零错误
    }
    cout << "new thread quit ..." << endl;
    return (void*)10;    
} 



void testThread2()  // test除零错误
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine1, (void*)"thread 1");  // 创建线程

    while (true)
    {
        cout << "main: " << " running ..." << endl;
        sleep(1);
    }

    // 线程在创建并执行的时候，线程也是需要进行等待的，
    // 如果主线程如果不等待，即会引起类似于进程的僵尸问题，导致内存泄漏
    pthread_join(tid, nullptr); // 线程等待

}


///////////////////////////////////////////////////


// pthread_self()获取线程ID
void* threadRoutine2(void* args)
{
    int i = 0;
    int* data = new int[10]; 
    
    cout << "新线程： " << (char *)args << " running ...   ID:" << pthread_self() << endl;
    while (i++ < 10)
    {
        data[i] = i; // 写入i
    }
    cout << "new thread quit ..." << endl;
    
    //return (void*)10;    
    return (void*)data;    
} 


void testThread3()  // 线程等待 和 线程返回值
{
    pthread_t tid;
    void* ret = nullptr; // 用于接收返回值
    pthread_create(&tid, nullptr, threadRoutine2, (void*)"thread 1");  // 创建线程

    pthread_join(tid, (void**)&ret); // 线程等待
    
    //cout << (long long)ret << endl;  // 返回值10，强转为指针后，因为是64位机器，所以是8字节longlong型
    
    for (int i = 0; i < 10; i++)
    {
        cout << ((int*)ret)[i] << "  ID:" << pthread_self() << endl;  // 返回值int*数组
    }

    // 线程终止
    // 线程被取消，join的时候，退出码是-1 #define PTHREAD_CANCELED ((void *) -1)
    // pthread_exit()  // 在要终止的线程内部关闭
    // pthread_cancel()   // 在要终止的线程外部关闭
}



///////////////////////////////////////////////

// pthread_self()获取线程ID
void* threadRoutine3(void* args)
{
    pthread_detach(pthread_self());   // 线程分离 ,分离后主线程不能join等待

    int i = 0;
    int* data = new int[10]; 
    
    cout << "新线程： " << (char *)args << " running ...   ID:" << pthread_self() << endl;
    while (i++ < 10)
    {
        data[i] = i; // 写入i
    }
    cout << "new thread quit ..." << endl;
    pthread_exit((void*)11);  // 线程内部关闭, 返回err码 11
    
    //return (void*)10;    
    return (void*)data;    
} 

void testThread4()  // 线程分离
{
    pthread_t tid;
    void* ret = nullptr; // 用于接收返回值
    pthread_create(&tid, nullptr, threadRoutine3, (void*)"thread 1");  // 创建线程

    pthread_join(tid, (void**)&ret); // 线程等待   
}


////////////////////////////////////////////////


int tickets = 10000; // 在并发访问的时候，导致了我们数据不一致的问题！

void *getTickets(void *args)
{
    (void)args;
    while(true)  // 抢票
    {
        if(tickets > 0)
        {
            usleep(1000);
            printf("%p: %d\n", pthread_self(), tickets);
            tickets--;
        }
        else
        {
            break;
        }
    }
    return nullptr;
}

void testThread5() // 并发导致的问题
{
    pthread_t t1,t2,t3;
    // 多线程抢票的逻辑
    pthread_create(&t1, nullptr, getTickets, nullptr);
    pthread_create(&t2, nullptr, getTickets, nullptr);
    pthread_create(&t3, nullptr, getTickets, nullptr);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
}