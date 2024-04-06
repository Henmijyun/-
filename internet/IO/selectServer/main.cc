//高级IO 多路转接

#include "selectServer.hpp"

#include <memory>


void sizeFd_set()
{
    // 因为fs_set是一个固定大小的位图，直接决定了select能同时关心fd的个数，是有上限的。
    // 每个位图是8个bit位，所以文件描述符集中的全部文件*8，就是可以控制的全部文件描述数量
    // 测试本环境下的数量：
    std::cout << sizeof(fd_set) * 8 <<std::endl;   // 1024
}

int main()
{
    std::unique_ptr<SelectServer> svr(new SelectServer());
    svr->Start();
    
    return 0;
}