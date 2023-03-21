#include <iostream>
#include <string>
#include <signal.h>

static void Usage(std::string proc)
{
    std::cout << "Usage:\r\n\t" << proc << " signumber processid" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    int signumber = atoi(argv[1]);   // 提取数字  string to int
    int procid = atoi(argv[2]);

    kill(procid, signumber);  // pid号 和 sig信号

    return 0;
}