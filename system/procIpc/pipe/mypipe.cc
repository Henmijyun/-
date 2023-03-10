
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    // 1. �����ܵ�
    int pipefd[2] = { 0 }; // pipefd[0(��ͣ�����)]�����ˣ� pipefd[1(�ֱʣ�д)]��д��
    int n = pipe(pipefd);  // �ܵ�����������Ϊ2λ������
    assert(n != -1);   
    (void)n;     // ����Ϊ����releaseģʽ�¼��پ�����assertֻ��debugģʽ����Ч 

#ifdef DEBUG
    cout << "pipefd[0]: " << pipefd[0] << endl;   // �ļ������� 3
    cout << "pipefd[1]: " << pipefd[1] << endl;   // �ļ������� 4
#endif

    //  2.�����ӽ���
    pid_t id = fork();
    assert(id != -1);
    if (id == 0)
    {
        // �ӽ��� - ��
        // 3. ��������ͨ�ŵ��ŵ���������д�룬�ӽ��̶�ȡ
        // 3.1 �ر��ӽ��̲���Ҫ��fd
        close(pipefd[1]);
        char buffer[1024 * 8];
        while (true)
        {
            // sleep(20);
            // д���һ����fdû�йرգ���������ݣ��Ͷ���û�����ݾ͵�
            // д���һ����fd�رգ���ȡ��һ����read�᷵��0����ʾ�������ļ��Ľ�β��
            ssize_t s = read(pipefd[0], buffer, sizeof(buffer) - 1);   // �����ݣ�����ֵΪ������������
            if (s > 0)
            {
                buffer[s] = 0;
                cout << "child get a message[" << getpid() << "] Father# " << buffer << endl;
            }
            else if (s == 0)
            {
                cout << "writer quit(father), me quit!!!" << endl;  // ȫ������
                break;
            }
        }
        // close(pipefd[0]);
        exit(0);
    }

    // ������ - д
    // 3. ��������ͨ�ŵ��ŵ�
    // 3.1 �رո����̲���Ҫ��fd
    close(pipefd[0]);
    string message = "���Ǹ����̣������ڸ��㷢��Ϣ";
    int count = 0;
    char send_buffer[1024 * 8];
    while (true)
    {
        // 3.2 ����һ���仯���ַ���
        snprintf(send_buffer, sizeof(send_buffer), "%s[%d] : %d", message.c_str(), getpid(), count++);
        
        // 3.3 д��
        write(pipefd[1], send_buffer, strlen(send_buffer));
        
        // 3.4 ����sleep
        sleep(1);
        cout << count << endl;
        if (count == 5)
        {
            cout << "writer quit(father)" << endl;
            break;
        }
    }

    close(pipefd[1]);
    pid_t ret = waitpid(id, nullptr, 0); 
    cout << "id : " << id << " ret : " << ret << endl;
    assert(ret > 0);
    (void)ret;


    return 0;
}



