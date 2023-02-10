#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{

    int a = 10;
    fscanf(stdin, "%d", &a); // 写
    printf("%d\n", a);

    char input[16];
    ssize_t s1 = read(0, input, sizeof(input));  // 通过 文件描述符0 向input中写入
    if (s1 > 0)
    {
        input[s1] = '\0';   // read函数返回值为实际读取的字符数，如果读到4，最后一个字符下标为3
        printf("%s\n", input);  // 所以在s1位置手动插入'\0'
    }

    fprintf(stdout, "hello stdout\n"); // 读
    const char* s2 = "hello 1\n";

    write(1, s2, strlen(s2)); // 通过 文件描述符1 把s读到屏幕

    // stdin, stdout, stderr -> FILE* -> 内部也有fd
    printf ("stdin: %d\n", stdin->_fileno);    // 0
    printf ("stdout: %d\n", stdout->_fileno);  // 1
    printf ("stderr: %d\n", stderr->_fileno);  // 2

    return 0; 
}




/*
int main()
{
    umask(0);  // 设置umask进程权限

    // int fd = open("log2.txt", O_WRONLY | O_CREAT, 0666); // （文件名， O_WRONLY 写 | O_CREAT 创建文件，权限0666） rw-rw-rw-
    // int fd = open("log2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666); // O_TRUNC 清空文件
    // int fd = open("log2.txt", O_WRONLY | O_CREAT | O_APPEND, 0666); // O_APPEND 追加
   
    int fd1 = open("log1.txt", O_WRONLY | O_CREAT | O_APPEND, 0666); // O_APPEND 追加
    printf ("open success, fd1: %d\n", fd1);
    int fd2 = open("log2.txt", O_WRONLY | O_CREAT | O_APPEND, 0666); // O_APPEND 追加
    printf ("open success, fd2: %d\n", fd2);
    int fd3 = open("log3.txt", O_WRONLY | O_CREAT | O_APPEND, 0666); // O_APPEND 追加
    printf ("open success, fd3: %d\n", fd3);
    int fd4 = open("log4.txt", O_WRONLY | O_CREAT | O_APPEND, 0666); // O_APPEND 追加
    printf ("open success, fd4: %d\n", fd4);
    
    // int fd = open("log2.txt", O_RDONLY); // O_RDONLY 读
   // if (fd < 0)
   // {
   //     perror("open");
   //     return 1;
   // }
   // printf ("open success, fd:%d\n", fd);

   // char buffer[128];  // 数组设置的大小，决定了能读多少个字符
   // memset(buffer, '\0', sizeof(buffer)); // 全部添加为'\0'，最后就会以'\0'结尾
   // read(fd, buffer, sizeof(buffer)); // （buffer为二进制，类型是void*，不会自动添加'\0'）
   // 
   // printf ("%s", buffer);
    
    // open success
    //const char* s = "hello write\n";
    //write(fd, s, strlen(s));      // 参数（文件，要写入的字符串，字符个数）

    close(fd1); // 关闭文件
    close(fd2); // 关闭文件
    close(fd3); // 关闭文件
    close(fd4); // 关闭文件

    return 0;
}

*/






//// 用int中不重复的一个bit，标识一种状态（类似于位图） 
//#define ONE 0x1    // 0000 0001
//#define TWO 0x2    // 0000 0010
//#define THREE 0x4  // 0000 0100
//
//void show(int flags)
//{
//    if (flags & ONE)
//        printf("hello one\n");
//    if (flags & TWO)
//        printf("hello two\n");
//    if (flags & THREE)
//        printf("hello three\n");
//}
//
//int main()
//{
//    show(ONE);
//    printf("----------------------\n");
//    
//    show(TWO);
//    printf("----------------------\n");
//    
//    show(ONE | TWO);     // 0000 0001 | 0000 0010 -> 0000 0011 按位或
//    printf("----------------------\n");
//    
//    show(ONE | TWO | THREE); // 0000 0111
//    printf("----------------------\n");
//    
//    show(ONE | THREE); // 0000 0101
//    printf("----------------------\n");
//    return 0;
//}
