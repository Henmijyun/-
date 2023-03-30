#include "mysignal.hpp"

int main()
{
    //getSig();  // 1  获取信号

    //captureSig(); // 2 观察阻塞信号集block变化的过程 kill -2 

    blockSig();  // 3 把1~31信号 全部添加到阻塞信号集block 观察是否能全部阻塞

    return 0;
}