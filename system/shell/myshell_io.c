#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define NUM 1024
#define SIZE 32
#define SEP " " 

// 保存完整的命令行字符串
char cmd_line[NUM];

// 保存打散后的命令行字符串
char* g_argv[SIZE];

// 写一个环境变量的buffer，用于测试
char g_myval[64];

#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3
#define NONO_REDIR 0

int redir_status;  // 全局int 用于定义IO的文件描述符fd

char* CheckRedir(char* start)
{
    assert(start);

    char* end = start + strlen(start) - 1;  // ls -a -l\0 指向\0前面最后一个有效字符的位置
    while (end >= start)
    {
        if (*end == '>')
        {
            // ls -a -l>log.txt
            // ls -a -l>>log.txt
            if (*(end-1) == '>')
            {
                // >>追加
                redir_status = APPEND_REDIR;  // 追加重定向
                *(end-1) = '\0';
                end++;
                break;
            } 
            redir_status = OUTPUT_REDIR;  // 输出重定向
            *end = '\0';
            end++;
            break;
        }
        else if (*end == '<')
        {
            // cat<myfile.txt 输入
            redir_status = INPUT_REDIR; // 输入重定向
            *end = '\0';
            end++;
            break;
        }
        else 
        {
            end--;
        }
    }

    if (end >= start)
    {
        return end; // 要打开的文件
    }
    else 
    {
        return NULL;
    }
}

// shell 运行原理：通过让子进程执行命令，父进程等待&&解析命令
int main()
{
    extern char** environ;   // 全局二级指针 指向环境变量

    // 0、命令行解释器shell，一定是一个常驻内存的进程，不退出
    while (1)
    {
        // 1、打印提示信息[skk@localhost myshell]#
        printf("[skk@我的主机 myshell]# ");
        fflush(stdout);   // 提取缓冲区的提示信息
        memset(cmd_line, '\0', sizeof cmd_line); // 全部改成'\0'

        // 2、获取用户的键盘输入[输入的是各种指令和选项："ls -a -l -i"]
        // "ls -a -l>log.txt" 重定向
        // "ls -a -l>>log.txt"
        // "ls -a -l<log.txt" 
        if (fgets(cmd_line, sizeof cmd_line, stdin) == NULL)
        {
            continue; 
        }
        cmd_line[strlen(cmd_line) - 1] = '\0'; // "ls -a -l -i \n\0" 需要把\n也改\0 

        // 2.1、分析是否有重定向，"ls -a -l>log,txt" -> "ls -a -l\0log.txt"
        char* sep = CheckRedir(cmd_line);

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
       

        // 4、内置命令，让父进程(shell)自己去执行的命令，叫做内置命令 或 内建命令。 (cd ecport 命令)
        //  内建命令本质其实是shell中的一个函数调用
        
        // export myval=666 设置新的环境变量，子进程会继承
        if (strcmp(g_argv[0], "export") == 0 && g_argv[1] != NULL)
        {
            strcpy(g_myval, g_argv[1]);
            int ret = putenv(g_myval);  // putenv函数，添加一个环境变量
            if (ret == 0) 
            {
                printf ("%s export success\n", g_argv[1]);  // 父进程是否导入成功
            }
            // for (int i = 0; environ[i]; i++)
            //      printf ("%d: %s\n", i, environ[i]);
            
            continue; 
        }

        // cd 移动路径
        if (strcmp(g_argv[0], "cd") == 0) // not child execute, father execute
        {
            if (g_argv[1] != NULL)
            {
                chdir(g_argv[1]); // chdir函数，移动到字符串中的位置 cd path/code/、 cd ..  
            }
            continue;
        }

        // 5、fork()
        pid_t id = fork();
        if (id == 0) // child
        {
            if (sep != NULL)
            {
                int fd = -1;
                // 说明命令曾经有重定向
                switch(redir_status)
                {
                    case INPUT_REDIR:
                        fd = open(sep, O_RDONLY); 
                        dup2(fd, 0);
                        break;
                    case OUTPUT_REDIR:
                        fd = open(sep, O_WRONLY | O_TRUNC | O_CREAT, 0666); 
                        dup2(fd, 1);
                        break;
                    case APPEND_REDIR:
                        fd = open(sep, O_WRONLY | O_APPEND | O_CREAT, 0666); // 追加
                        dup2(fd, 1);
                        break;
                    default:
                        printf("bug?\n");
                        break;
                }
                
            }

            //printf("下面功能是由子进程所进程的\n");
            //printf("child, MYVAL: %s\n", getenv("MYVAL"));
            //printf("child, PATH: %s\n", getenv("PATH"));
            execvp(g_argv[0], g_argv); // 替换，参数("ls", "ls -a -l -i")
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
       









