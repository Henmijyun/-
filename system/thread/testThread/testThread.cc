#include "testThread.hpp"

int main()
{
    // testThread1(); // 创建多线程

    // testThread2();   // 多线程报错的时候，整个进程退出

    // testThread3();  // 线程等待 和 线程返回值  线程终止

    // testThread4();  // 线程分离

    testThread5(); // 并发导致的问题

    return 0;
}