#include "mysignal.hpp"

int main()
{
    //getSig();  // test_1  获取信号

    //captureSig(); // test_2 观察阻塞信号集block变化的过程 kill -2 

    // blockSig();  // test_3 把1~31信号 全部添加到阻塞信号集block 观察是否能全部阻塞

    
    // sigProcOne();  // test_4 在任何时刻，只处理一层信号
    // 需要在make中加-fpermissive  忽略丢失精度的警告

    // test_5 编译器优化导致的信号无法接收问题
    // 需要在make中加-O3  使优化程度加大，使得编译时检测到全局变量不变，而直接写入寄存器中
    // 运行时进程不再访问内存，而是读寄存器中默认优化的值
    // 解决方法：volatile关键字
    //testVolatile();

    // test_6 子进程退出，默认发送17号SIGCHLD信号
    // 父进程接收等待，可以通过遍历vector和waitpid函数的-1参数，
    // 让它默认执行的不是系统的默认暂停，而是改为默认退出
    testSigchld();
    return 0;
}