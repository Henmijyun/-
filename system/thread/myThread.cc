#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <pthread.h>

using namespace std;

int x = 100;

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

int main()
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

    return 0;
}