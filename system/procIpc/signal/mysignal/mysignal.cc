#include "mysignal.hpp"

int main()
{
    //getSig();  // test_1  获取信号

    //captureSig(); // test_2 观察阻塞信号集block变化的过程 kill -2 

    // blockSig();  // test_3 把1~31信号 全部添加到阻塞信号集block 观察是否能全部阻塞

    
    sigProcOne();  // test_4 在任何时刻，只处理一层信号
    // 需要在make中加-fpermissive  忽略丢失精度的警告

    
    return 0;
}