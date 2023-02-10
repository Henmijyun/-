#include <stdio.h>
#include <unistd.h>
#include <string.h>

// myfile 一个文件名
int main(int argc, char* argv[])  // （参数数量，参数字符串）argv[0]是程序名 argv[1]是参数
{
    if (argc != 2)
    {
        printf("argv error!\n");
        return 1;
    }

    // FILE* fp = fopen("log.txt", "a"); // r只读，w清空再添加，a追加
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        //strerror
        perror("fopen");
        return 2;
    }

    // 进行文件操作
    // 按行读取
    char line[64];
    // fgets -> C -> s(string) -> 会自动在字符结尾添加\0
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // printf ("%s", line);
        fprintf(stdout, "%s", line);  // fprintf->stdout 
    }

   // const char* s1 = "hello fwrite \n"; // 字符串不用+1，\0是C语言的规定，文件不用遵循
   // fwrite(s1, strlen(s1), 1, fp);

   // const char* s2 = "hello fpeintf\n";
   // fprintf(fp, "%s", s2);

   // const char* s3 = "hello fputs\n";
   // fputs(s3, fp);

    fclose(fp);

    // while (1)
    // {
    //     sleep(1);
    // }

    return 0;
}



