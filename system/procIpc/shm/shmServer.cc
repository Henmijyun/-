#include "comm.hpp"

// 程序在加载的时候，会自动构建全局变量，就要调用该类的构造函数 -- 创建管道文件
// 程序退出的时候，自动调用析构函数，会自动删除管道文件
Init init; 


string TransToHex(key_t k)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "0x%x", k);  // 从数组中读数据，改为16进制
    return buffer;
}

int main()
{
    // 1.创建公共的key值
    key_t k = ftok(PATH_NAME, PROJ_ID);
    assert(k != -1);

    Log("create key done", Debug) << "server key : " << TransToHex(k) << endl;
    
    // 2.创建共享内存 -- 建议要创建一个全新的共享内存 -- 通信发起者
    int shmid = shmget(k, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    Log("create shm done", Debug) << " shmid : " << shmid << endl;
    
    // sleep(10);

    // 3. 将指定的共享内存，挂接到自己的地址空间
    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    Log("attach shm done", Debug) << " shmid : " << shmid << endl;

    // sleep(10);
    
    // 通信
    // 将共享内存当成一个大字符串进行读取
    // 结论1： 只要是通信双方使用shm，一方直接向共享内存中写入数据，另一方，就可以立马看到对方写入的数据。
    //         共享内存是所有进程间通信(IPC)，速度最快的！不需要过多的拷贝！！（不需要将数据给操作系统）
    // 结论2： 共享内存缺乏访问控制！会带来并发问题 【这里用管道来进行控制】

    int fd = OpenFIFO(FIFO_NAME, READ);
    for (;;)
    {
        Wait(fd);   // 等待

        // 临界区
        printf("%s\n", shmaddr);
        if (strcmp(shmaddr, "quit") == 0)
        {
            break;
        }
    }


    // 4.将指定的共享内存，从自己的地址空间中 去除掉 关联 （取消关联）
    int n = shmdt(shmaddr);
    assert(n != -1);
    (void)n;

    Log("detach shm success", Debug) << " shmid : " << shmid << endl;
    // sleep(10);

    // 5.删除共享内存，IPC_RMID即便是有进程和当下的shm挂接，依旧删除共享内存
    n = shmctl(shmid, IPC_RMID, nullptr);
    assert(n != -1);
    (void)n;
    
    Log("detach shm done", Debug) << " shmid : " << shmid << endl;

    return 0;
}
