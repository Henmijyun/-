
#include <stdio.h>
#include <unistd.h>

int main ()
{
    // 子进程创建失败例
    int max_cnt = 0;
    while (1)
    {
        pid_t id = fork();
        if (id < 0)
        {
            printf ("fork errer: %d\n", max_cnt);
            break;
        }
        else if (id == 0)
        {
            while (1)
            {
                sleep(1);
            }
            // exit(1);
        }
        else 
        {
            // father
        }
        max_cnt++;
    }
    return 0;
}



