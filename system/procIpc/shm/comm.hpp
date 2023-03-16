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
/*
#define FIFO_NAME "./fifo"   // 当前路径下

class Init
{
public:
    Init()
    {
        umask(0);
        int n = mkfifo(FIFO_NAME, 0666);
        assert(n == 0);
        (void)n;
        Log("create fifo success", Notice) << "\n";
    }

    
};
*/