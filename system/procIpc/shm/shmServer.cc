#include "comm.hpp"

string TransToHex(key_t k)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "0x%x", k);  // �������ж����ݣ���Ϊ16����
    return buffer;
}

int main()
{
    // 1.����������keyֵ
    key_t k = ftok(PATH_NAME, PROJ_ID);
    assert(k != -1);

    Log("create key done", Debug) << "server key : " << TransToHex(k) << endl;
    
    // 2.���������ڴ� -- ����Ҫ����һ��ȫ�µĹ����ڴ� -- ͨ�ŷ�����
    int shmid = shmget(k, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    Log("create shm done", Debug) << " shmid : " << shmid << endl;
    
    sleep(10);

    // 3. ��ָ���Ĺ����ڴ棬�ҽӵ��Լ��ĵ�ַ�ռ�
    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    Log("attach shm done", Debug) << " shmid : " << shmid << endl;

    sleep(10);
    
    // �������ͨ�ŵ��߼���


    // 4.��ָ���Ĺ����ڴ棬���Լ��ĵ�ַ�ռ���ȥ����
    int n = shmdt(shmaddr);
    assert(n != -1);
    (void)n;

    Log("detach shm success", Debug) << " shmid : " << shmid << endl;
    sleep(10);

    // 5.ɾ�������ڴ棬IPC_RMID�������н��̺͵��µ�shm�ҽӣ�����ɾ�������ڴ�
    n = shmctl(shmid, IPC_RMID, nullptr);
    assert(n != -1);
    (void)n;
    
    Log("detach shm done", Debug) << " shmid : " << shmid << endl;

    return 0;
}
