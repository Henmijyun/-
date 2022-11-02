#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM 1024
#define SIZE 32
#define SEP " " 

// 保存完整的命令行字符串
char cmd_line[NUM];
// 保存打散后的命令行字符串
char* g_argv[SIZE];

// shell 运行原理：通过让子进程执行命令，父进程等待&&解析命令
int main()
{
    // 0、命令行解释器shell，一定是一个常驻内存的进程，不退出
    while (1)
    {
        // 1、打印提示信息[skk@localhost myshell]#
        printf("[skk@localhost myshell]# ");
        fflush(stdout);   // 提取缓冲区的提示信息
        memset(cmd_line, '\0', sizeof cmd_line); // 全部改成'\0'

        // 2、获取用户的键盘输入[输入的是各种指令和选项："ls -a -l -i"]
        if (fgets(cmd_line, sizeof cmd_line, stdin) == NULL)
        {
            continue; 
        }
        cmd_line[strlen(cmd_line) - 1] = '\0'; // "ls -a -l -i \n\0" 需要把\n也改\0

        // 3、命令行字符串解析："ls -a -l -i" -> "ls" "-a" "-l" "-i"
        g_argv[0] = strtok(cmd_line, SEP); // 第一次调用，要传入原始字符串
        int index = 1;
        if (strcmp(g_argv[0], "ll") == 0) // ll -> ls -l
        {
            g_argv[0] = "ls";
            g_argv[index++] = "-l";
        }
        if (strcmp(g_argv[0], "ls") == 0)
        {
            g_argv[index++] = "--color=auto"; // 调用ls时，加上颜色
        }

        while (g_argv[index++] = strtok(NULL, SEP)); // 第二次之后，继续解析原始字符串的话，传NULL即可
        
        // 4、内置命令，让父进程(shell)自己去执行的命令，叫做内置命令 或 内建命令。
        //  内建命令本质其实是shell中的一个函数调用
        if (strcmp(g_argv[0], "cd") == 0) // not child execute, father execute
        {
            if (g_argv[1] != NULL)
            {
                chdir(g_argv[1]); // 移动到字符串中的位置 cd path/code/、 cd ..  
            }
            continue;
        }

        // 5、fork()
        pid_t id = fork();
        if (id == 0) // child
        {
            printf("下面功能是由子进程所进程的\n");
            execvp(g_argv[0], g_argv); // ls ls -a -l -i
            exit(1);
        }
        else if (id < 0)
        {
            perror("fork:"); // 生成child失败
            exit(1);
        }
        else // father 
        {
            int status = 0;
            pid_t ret = waitpid(id, &status, 0);  // 阻塞等待
            if (ret > 0)
            {
                // exit时，打印退出码
                printf("exit code:%d\n",WEXITSTATUS(status));
            }
        }

    }
    return 0;
}
       









