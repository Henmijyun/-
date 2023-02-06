#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM 16

const char* myfile = "./mycmd"; // 绝对路径 或者 相对路径

int main(int argc, char* argv[], char* env[])
{
    char* const _env[NUM] = {
        "MY_TEST_VAL=11223344",
        NULL 
    };

    pid_t id = fork();
    if (id == 0)
    {
        // child
        printf("子进程开始运行，pid:%d\n", getpid());
        sleep(3);
        char* const _argv[NUM]={
            "ls",
            "-a",
            "-l",
            "-i",
            NULL 
        };
        
        // 用法：
        
        //execle(myfile, "mycmd", "-a", NULL, env);
        execle(myfile, "mycmd", "-a", NULL, _env);   // 路径 程序名 参数 （NULL参数结尾） 环境变量
        //
        //execlp("./test.py", "test.py", NULL);         // 加了chmod +x test.py 指令，使系统自动调用python的解析器
        //execlp("bash", "bash", "test.sh", NULL);
        //execlp("python", "python", "test.py", NULL);   // 不加chmod +x test.py
        //execl(myfile, "mycmd", "-b", NULL);
        //execl("/usr/bin/ls", "ls", "-a", "-l", NULL);
        //execv("/usr/bin/ls", _argv); //和上面的execl只有传参方式的区别
        //execlp("ls", "ls", "-a", "-l", NULL);
        //execvp("ls", _argv);
        
        exit(1);
    }
    else 
    {
        // father
        printf("父进程开始运行，pid:%d\n", getpid());
        int status = 0;
        pid_t id = waitpid(-1, &status, 0); // 阻塞等待
        if (id > 0)
        {
            printf("wait success, exit code:%d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}


