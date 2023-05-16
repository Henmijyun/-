#include "ringQueue.hpp"
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>

void* consumer(void* args) // 消费者
{
    RingQueue<int>* rq = (RingQueue<int>*)args;
    while (true)
    {
        sleep(1);

        // 1.从环形队列中获取任务或者数据
        int x = 10;

        // 2.进行处理--时间会消耗
        rq->pop(&x); 
        std::cout << "消费： " << x << std::endl;
    }
}

void* productor(void* args) // 生产者
{
    RingQueue<int>* rq = (RingQueue<int>*)args;
    while (true)
    {
        //sleep(3);

        // 1.构建数据或者任务对象--外部来的--时间会消耗
        int x = rand() % 100 + 1;

        // 2.推送到环形队列中
        rq->push(x); // 完成生产的过程
        std::cout << "生产： " << x << std::endl;

    }
}


int main()
{
    srand((uint64_t)time(nullptr) ^ getpid() ^ 0x1234);  // 随机数

    RingQueue<int>* rq = new RingQueue<int>(10);
    // rq->debug();

    pthread_t c[3], p[2];
    pthread_create(c, nullptr, consumer, (void*)rq);
    pthread_create(c + 1, nullptr, consumer, (void*)rq);
    pthread_create(c + 2, nullptr, consumer, (void*)rq);

    pthread_create(p, nullptr, productor, (void*)rq);
    pthread_create(p + 1, nullptr, productor, (void*)rq);


    for (int i = 0; i < 3; i++)
    {
        pthread_join(c[i], nullptr);
    }
    for (int i = 0; i < 2; i++)
    {
        pthread_join(p[i], nullptr);
    }

    return 0;
}

