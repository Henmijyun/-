#include "Task.hpp"

#define PROCESS_NUM 5

int waitCommand(int waitFd, bool &quit)  // ����Է����������Ǿ�����
{
    uint32_t command = 0;  // �̶���С32bit
    ssize_t s = read(waitFd, &command, sizeof(command));  // ��������ֵΪ����������
    if (s == 0)
    {
        quit = true;
        return -1;
    }
    assert(s == sizeof(uint32_t));
    return command;
}

void sendAndWakeup(pid_t who, int fd, uint32_t command)
{
    write(fd, &command, sizeof(command));
    cout << "main process: call process " << who << " execute " << desc[command] << " through " << fd << endl;
}

int main()
{
    // �����й���fd�Ĵ�����һ��С����
    load();

    // pid: pipefd  �����б�
    vector<pair<pid_t, int>> slots;

    // �ȴ����������
    for (int i = 0; i < PROCESS_NUM; i++)
    {
        // �����ܵ�
        int pipefd[2] = { 0 };
        int n = pipe(pipefd);  // �ܵ���ϵͳ����
        assert(n == 0);   // �ɹ�����0��ʧ�ܷ���-1
        (void)n;

        pid_t id = fork();
        assert(id != -1);
        
        // �ӽ��������������ж�ȡ
        if (id == 0)
        {
            // �ر�д��
            close(pipefd[1]);

            // child
            while (true)
            {
                // pipefd[0]
                // ������
                bool quit = false;
                int command = waitCommand(pipefd[0], quit); // ����Է����������Ǿ�����
                if (quit)
                {
                    break;
                }

                // ִ�ж�Ӧ������
                if (command >= 0 && command < handlerSize())
                {
                    callbacks[command]();  // ���ö�Ӧ�±�ĺ���
                }
                else
                {
                    cout << "�Ƿ�command: " << command << endl;
                }
            }
            exit(1);
        }
        
        // father, ����д�룬�رն���
        close(pipefd[0]);
        slots.push_back(pair<pid_t, int>(id, pipefd[1]));
    }

    // �������ɷ�����
    srand((unsigned long)time(nullptr) ^ getpid() ^ 2135123153L);  // ������Դ�����
    while (true)
    {
        // �������/��������
        // ѡ��һ��������������Ǵ�������������,����Ҫ�������
        int command = rand() % handlerSize();

        // ѡ��һ�����̣�����������ķ�ʽ��ѡ�����ȥ��������������ʽ�ĸ��ؾ���
        int choice = rand() % slots.size();

        // �������ָ���Ľ���
        sendAndWakeup(slots[choice].first, slots[choice].second, command);
        sleep(1);

        // ����ѡ���ͣ�
        // int select, command;
        // cout << "#######################################" << endl;
        // cout << "#  1.show funcitons    2.send command #" << endl;
        // cout << "#######################################" << endl;
        // cout << "Please Select> ";
        // cin >> select;
        // if (select == 1)
        //     showHandler();
        // else if (select == 2)
        // {
        //      cout << "Enter Your Command> ";
        //      // ѡ������
        //      cin >> command;
        //      // ѡ�����
        //      int choice = rand() % slots.size();
        //      // �������ָ���Ľ���
        //      sendAndWakeup(slots[choice].first, slots[choice].second, command);
        // }
        // else
        // {
        // }
    }

    // �ر�fd, ���е��ӽ��̶����˳�
    for (const auto &slot : slots)
    {
        close(slot.second);
    }

    // �������е��ӽ�����Ϣ
    for (const auto &slot : slots)
    {
        waitpid(slot.first, nullptr, 0);
    }

    return 0;
}