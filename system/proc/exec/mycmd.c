#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ./mycmd -a/-b/-c..
int main(int argc, char* argv[])
{
    if (argc != 2) // 传入参数的数量
    {
        printf("can not execute!\n");
        exit(1);
    }

    // 如果带e，传了环境变量的话，打印环境变量
    printf("获取环境变量：MY_TEST_VAL：%s\n", getenv("MY_TEST_VAL"));

    if (strcmp(argv[1], "-a") == 0)
    {
        printf("hello a!\n");
    }
    else if (strcmp(argv[1], "-b") == 0)
    {
        printf("hello b!\n");
    }
    else 
    {
        printf("default!\n");
    }
    return 0;
}



