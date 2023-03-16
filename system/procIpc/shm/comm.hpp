#pragma once

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include <fcntl.h>
#include "Log.hpp"


using namespace std;  // 不推荐

#define PATH_NAME "/home/caojianxi/code"
#define PROJ_ID 0x66
#define SHM_SIZE 4096   // 共享内存的大小，最好是 页 (PAGE: 4096) 的整数倍

#define FIFO_NAME "./fifo"   // 命名管道路径

class Init
{
public:
    Init()
    {
        umask(0);  // 该程序创建的文件，权限全部为0的补码 -> 1
        int n = mkfifo(FIFO_NAME, 0666);   // 创建管道文件
        assert(n == 0);
        (void)n;
        Log("create fifo success", Notice) << endl;
    }

    ~Init()
    {
        unlink(FIFO_NAME);  // 删除管道文件
        Log("remove fifo success", Notice) << endl;
    }
};

#define READ O_RDONLY    // 读
#define WRITE O_WRONLY    // 写

int OpenFIFO(string pathname, int flags)   // 打开文件 读/写
{
    int fd = open(pathname.c_str(), flags);
    assert(fd >= 0);
    return fd;
}

void Wait(int fd)  // 等待
{
    Log("等待中...", Notice) << endl;

    uint32_t temp = 0;
    ssize_t s = read(fd, &temp, sizeof(uint32_t));
    assert(s == sizeof(uint32_t));
    (void)s;
}

void Signal(int fd)   // 唤醒
{
    uint32_t temp = 1;
    ssize_t s = write(fd, &temp, sizeof(uint32_t));
    assert(s == sizeof(uint32_t));
    (void)s;

    Log("唤醒中...", Notice) << endl;
}

void CloseFIFO(int fd)
{
    close(fd);
}

