
#include <stdio.h>
#include <unistd.h>     // fork  getpid getppid _exit
#include <sys/types.h>  // 进程PID数据类型 pid_t
#include <sys/wait.h>   // wait waitpid 
#include <stdlib.h>    // exit 

#define N 2

void fun1()
{
    printf("临时任务1\n");
}

void fun2()
{
    printf("临时任务2\n");
}
void (*pfArr[N])() ={ 0 };

int main()
{
    pid_t id = fork();
    if (id < 0)
    {
        perror("fork::");
        exit(1); // 运行完毕，结果不正确
    }
    else if (id == 0)
    {
        // 子进程
        int quit = 3;
        while (quit--)
        {
            printf("我是子进程：PID %d, PPID %d\n", getpid(), getppid());
            sleep(1);
        }

        exit(15);  // 15 仅仅用于测试
    }
    else 
    {
        // 父进程
        printf("我是父进程:PID %d, PPID %d\n", getpid(), getppid());
       
        int quit = 0;
        while (!quit)
        {

            int status = 0; // 接收结束返回的子进程PID
            // pid_t ret = wait(&status); // 阻塞式等待
        
            pid_t result = waitpid(id, &status, WNOHANG); // 阻塞式等待
            if (result > 0)
            {
                // printf("子进程回收成功 ret:%d, 子进程收到的信号编号 %d，子进程的退出码 %d\n", ret, (status >> 8) & 0xFF, status & 0x7F);
            
                if (WIFEXITED(status))  // 子进程正常结束 WIFEXITED为真
                {
                    // 子进程正常结束
                    printf("子进程执行完毕，子进程的退出码：%d\n", WEXITSTATUS(status));
                }
                else   // 子进程异常退出时，WIFEXITED为0 ,
                {   
                    printf("子进程异常退出：%d,错误码为 %d\n", WIFEXITED(status), status&0x7F);
                }
                quit = 1;
            }
            else if(result == 0) // 非阻塞等待 WNOHANG 
            {
                 // 等待成功 && 但子进程并未退出
                 printf("子进程还在运行中,暂时还没有退出,父进程先等等,可以先做别的事\n");
                 if (pfArr[0] == 0)
                 {
                     pfArr[0] = fun1;
                     pfArr[1] = fun2;
                 }
                 int i = 0;
                 for (i = 0;i < N; ++i)
                 {
                     pfArr[i]();
                 }
                 sleep(1);
            }
            else 
            {
                // 等待失败
                printf("wait失败!\n");
                quit = 1;
            }

        }
    }
    return 0;
}



