#include "comm.hpp"

int main()
{
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

    sleep(10);

    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    if (shmaddr == nullptr)
    {
        Log("attach shm failed", Error) << " client key : " << k << endl; 
        exit(3);
    }
    Log("attach shm success", Debug) << " client key : " << k << endl; 
    sleep(10);

    // 使用
    

    // 关联
    int n = shmdt(shmaddr);
    assert(n != -1);
    (void)n; 

    Log("detach shm success", Debug) << " client key : " << k << endl; 
    sleep(10);

    // client 中，不需要chmctl删除，因为管理共享文件是shmServer的工作
    return 0;
}
