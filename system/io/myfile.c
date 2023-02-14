// 实现一个用户层缓冲区

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define NUM 1024

struct MyFILE
{
    int fd;
    char buffer[NUM];
    int end;  // 当前缓冲区的结尾
};

typedef struct MyFILE MyFILE;

MyFILE* fopen_(const char* pathname, const char* mode)  // 打开文件
{
    assert(pathname);
    assert(mode);

    MyFILE* fp = NULL;

    if (strcmp(mode, "r") == 0) // 比较mode字符串
    {}
    else if (strcmp(mode, "r+") == 0)
    {}
    else if (strcmp(mode, "w") == 0)
    {
        int fd = open(pathname, O_WRONLY | O_TRUNC | O_CREAT, 0666);
        if (fd >= 0) 
        {
            // 成功
            fp = (MyFILE*)malloc(sizeof(MyFILE));
            memset(fp, 0, sizeof(MyFILE)); // 全部变0
            fp->fd = fd;
        }
    }
    else if (strcmp(mode, "w+") == 0)
    {}
    else if (strcmp(mode, "a") == 0)
    {}
    else if (strcmp(mode, "a+") == 0)
    {}
    else
    {
        // 什么都不做
    }

    return fp;
}

// C标准库中实现的
void fputs_(const char* message, MyFILE* fp)  // 在特定流中，打印
{
    assert(message);
    assert(fp);

    strcpy(fp->buffer + fp->end, message); // 找到buffer结尾\0的位置，然后拷贝message
    fp->end += strlen(message);

    // for debug
    printf("%s\n", fp->buffer);

    // 暂时不刷新,最后策略是谁来执行的呢?? 用户通过执行C标准库中的代码逻辑，来完成刷新动作
    // 效率提高体现在哪里？ 因为C提供了缓冲区，通过策略，减少了IO执行的次数（不是减少数据量）
    if (fp->fd == 0)
    {
        // 标准输入
    }
    else if (fp->fd == 1)
    {
        // 标准输出
        if (fp->buffer[fp->end-1] == '\n')
        {
            // fprintf(stderr, "fflush: %s", fp->buffer); // 2
            write(fp->fd, fp->buffer, fp->end);
            fp->end = 0;
        }
    }
    else if (fp->fd == 2)
    {
        // 标准错误
    }
    else 
    {
        // 其他文件
    }
}

void fflush_(MyFILE* fp)  // 立即刷新缓冲区
{
    assert(fp);

    if (fp->end != 0)
    {
        // 暂且认为刷新了 -- 其实是把数据写到了内核
        write(fp->fd, fp->buffer, fp->end);
        syncfs(fp->fd);  // 将内核数据写入到磁盘
        fp->end = 0;
    }
}

void fclose_(MyFILE* fp)  // 关闭文件流
{
    assert(fp);

    fflush_(fp);
    close(fp->fd);
    free(fp);
}

int main()
{
    // close(1);
    MyFILE* fp = fopen_("./log.txt", "w");
    if (fp == NULL)
    {
        printf("open file error\n");
        return 1;
    }

    fputs_("one: hello world", fp); // 在特定的流当中，打印特定字符串
   
    fork();  // 创建子进程

    fclose_(fp);

    return  0;
}
























// myfile helloworld
//int main(int argc, char* argv[])
//int main()
//{
//    // C语言提供的
//    printf("hello printf\n");
//    fprintf(stdout, "hello fprintf\n");
//    const char* s = "hello fputs\n";
//    fputs(s, stdout);
//
//    // OS提供的
//    const char* ss = "hello write\n";
//    write(1, ss, strlen(ss));
//
//    // 注意，我们是在最后调用的fork，上面的函数已经被执行完了。
//    fork(); // 创建子进程
//
//
//
//
//
//
//
//
//
//    // close(2); // fd分配规则是：最小的，没有被占用的文件描述符
//   
//    //if (argc != 2)
//    //{
//    //    return 2;
//    //}
//
//    //// int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
//    //int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);   
//    //if (fd < 0)
//    //{
//    //    perror("open");
//    //    return 1;
//    //}
//
//    //dup2(fd, 1);
//    //fprintf(stdout, "%s\n", argv[1]); // stdout -> 1 -> 显示器
//
//
//
//
//
//   // printf ("fd: %d\n", fd);
//   // printf ("fd: %d\n", fd);
//   // printf ("fd: %d\n", fd);
//   // printf ("fd: %d\n", fd);
//   // printf ("fd: %d\n", fd);
//
//    // close(fd);
//    return 0;
//}
