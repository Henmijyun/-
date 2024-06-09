#pragma once

#include <iostream>

#include <sys/epoll.h>
#include <unistd.h>

class Epoll
{
public:
    static const int gsize = 256;   // 无用，只要大于0即可
public:
    // 创建epoll模型
    static int Create()
    {
        int epfd = epoll_create(gsize);
        if (epfd > 0)
        {
            return epfd;
        }
        exit(5);   // 创建epoll模型失败
    }    

    // 添加epfd到红黑树
    static bool CtlEpoll(int epfd, int oper, int sock, uint32_t events)
    {
        static epoll_event ev;
        ev.events = events;
        ev.data.fd = sock;
        int n = epoll_ctl(epfd, oper, sock, &ev);
        return n == 0;
    }

    // 从就绪列表中 获取就绪事件到revs数组
    static int WaitEpoll(int epfd, struct epoll_event revs[], int num, int timeout)
    {
        return epoll_wait(epfd, revs, num, timeout);
    }
};
