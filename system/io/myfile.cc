#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


// 模拟封装一个perror
void myperror(const char* msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
}


int main()
{
    int fd = open("log.txt", O_RDONLY);
    if (fd < 0)
    {
        myperror("open");
        return 1;
    }


    //// stdout -> 1
    //printf("hello printf 1\n");
    //fprintf(stdout, "hello fprintf 1\n");

    //// stderr -> 2
    //errno = 5;
    //perror("hello perror 2");
    //
    //const char* s1 = "hello write 1\n";
    //write(1, s1, strlen(s1));

    //const char* s2 = "hello write 2\n";
    //write(2, s2, strlen(s2));

    //// cout -> 1 
    //std::cout << "hello cout 1" << std::endl;
    //// cout -> 2
    //std::cerr << "hello cerr 2" << std::endl;

    return 0;
}




























//int main()
//{
//    close(1); // 这里关闭1标准输出
//    int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
//    if (fd < 0)
//    {
//        perror("open");
//        return 1;
//    }
//
//    printf("hello world: %d\n", fd); // stdout -> 1 -> 数据会暂时在stdout的缓冲区中
//    // fflush(stdout); // 刷新缓冲区
//    // const char* msg = "hello world\n";
//    // write(fd, msg, strlen(msg));
//    
//    close(fd); // fd -> 1 ：数据在缓冲区中，但是对应的fd先关了，数据变无法刷新了。
//
//    return 0;
//}


