#include "comm.hpp"

int main()
{
    Log("child pid is : ", Debug) << getpid() << endl;

    // 用ftok函数生成相同的key值
    key_t k = ftok(PATH_NAME, PROJ_ID);
    if (k < 0)
    {
        Log("create key failed", Error) << " client key : " << k << endl; 
        exit(1);
    }
    Log("create key done", Debug) << " client key : " << k << endl; 

    // 获取共享内存
    int shmid = shmget(k, SHM_SIZE, 0);
    if (shmid < 0)
    {
        Log("create shm failed", Error) << " client key : " << k << endl; 
        exit(2);
    }
    Log("create key success", Debug) << " client key : " << k << endl; 

    // sleep(10);

    // 将共享内存 挂接到 自己的地址空间
    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    if (shmaddr == nullptr)
    {
        Log("attach shm failed", Error) << " client key : " << k << endl; 
        exit(3);
    }
    Log("attach shm success", Debug) << " client key : " << k << endl; 
    // sleep(10);

    // 使用
    int fd = OpenFIFO(FIFO_NAME, WRITE);
    while (true)
    {
        ssize_t s = read(0, shmaddr, SHM_SIZE-1);  // 返回值为读到的数据量
        if (s > 0)
        {
            shmaddr[s-1] = 0;  // s-1下标，是有效字符最后一位，则回车\n
            Signal(fd);   // 唤醒
            
            if (strcmp(shmaddr, "quit") == 0)
            {
                break;
            }
        }
    }

    CloseFIFO(fd);
    // char a = 'a';
    // for(; a <= 'z'; a++)
    // {
    //     shmaddr[a-'a'] = a;
    //     // 我们是每一次都向shmaddr[共享内存的起始地址]写入
    //     // snprintf(shmaddr, SHM_SIZE - 1,\
    //     //     "hello server, 我是其他进程,我的pid: %d, inc: %c\n",\
    //     //     getpid(), a);
    //     sleep(5);
    // }



    // 取消与共享内存的关联
    int n = shmdt(shmaddr);
    assert(n != -1);
    (void)n; 

    Log("detach shm success", Debug) << " client key : " << k << endl; 
    // sleep(10);

    // client 中，不需要chmctl删除，因为管理共享文件是shmServer的工作
    return 0;
}
