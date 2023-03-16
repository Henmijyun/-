#include "comm.hpp"

// �����ڼ��ص�ʱ�򣬻��Զ�����ȫ�ֱ�������Ҫ���ø���Ĺ��캯�� -- �����ܵ��ļ�
// �����˳���ʱ���Զ������������������Զ�ɾ���ܵ��ļ�
Init init; 


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
    
    // sleep(10);

    // 3. ��ָ���Ĺ����ڴ棬�ҽӵ��Լ��ĵ�ַ�ռ�
    char* shmaddr = (char*)shmat(shmid, nullptr, 0);
    Log("attach shm done", Debug) << " shmid : " << shmid << endl;

    // sleep(10);
    
    // ͨ��
    // �������ڴ浱��һ�����ַ������ж�ȡ
    // ����1�� ֻҪ��ͨ��˫��ʹ��shm��һ��ֱ�������ڴ���д�����ݣ���һ�����Ϳ����������Է�д������ݡ�
    //         �����ڴ������н��̼�ͨ��(IPC)���ٶ����ģ�����Ҫ����Ŀ�������������Ҫ�����ݸ�����ϵͳ��
    // ����2�� �����ڴ�ȱ�����ʿ��ƣ�������������� �������ùܵ������п��ơ�

    int fd = OpenFIFO(FIFO_NAME, READ);
    for (;;)
    {
        Wait(fd);   // �ȴ�

        // �ٽ���
        printf("%s\n", shmaddr);
        if (strcmp(shmaddr, "quit") == 0)
        {
            break;
        }
    }


    // 4.��ָ���Ĺ����ڴ棬���Լ��ĵ�ַ�ռ��� ȥ���� ���� ��ȡ��������
    int n = shmdt(shmaddr);
    assert(n != -1);
    (void)n;

    Log("detach shm success", Debug) << " shmid : " << shmid << endl;
    // sleep(10);

    // 5.ɾ�������ڴ棬IPC_RMID�������н��̺͵��µ�shm�ҽӣ�����ɾ�������ڴ�
    n = shmctl(shmid, IPC_RMID, nullptr);
    assert(n != -1);
    (void)n;
    
    Log("detach shm done", Debug) << " shmid : " << shmid << endl;

    return 0;
}
