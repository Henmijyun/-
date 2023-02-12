#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// myfile helloworld
int main(int argc, char* argv[])
{
    // close(2); // fd分配规则是：最小的，没有被占用的文件描述符
   
    if (argc != 2)
    {
        return 2;
    }

    // int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);   
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    dup2(fd, 1);
    fprintf(stdout, "%s\n", argv[1]); // stdout -> 1 -> 显示器





   // printf ("fd: %d\n", fd);
   // printf ("fd: %d\n", fd);
   // printf ("fd: %d\n", fd);
   // printf ("fd: %d\n", fd);
   // printf ("fd: %d\n", fd);

    close(fd);
    return 0;
}
