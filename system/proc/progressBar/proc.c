#include <stdio.h>
#include <string.h>
#include <unistd.h> //休眠函数的头文件

#define NUM 102

int main()
{
    char bar[NUM];
    int cnt = 0;
    const char* lable = "|/-\\"; //4个符号
    memset(bar, 0, sizeof(bar));//数组全部元素赋值0
    
    while (cnt <= 50)
    {
        printf("[%-50s][%d%%]%c\r", bar, cnt*2, lable[cnt%4]);
        bar[cnt++] = '#'; //每次把一个0改成#
        fflush(stdout); //刷新缓冲区
        //sleep(1); //休眠3秒
        usleep(30000); //休眠，以毫秒为单位
    }
    printf("\n");
    
    return 0;
}

