#pragma once

#include <iostream>
#include <sys/epoll.h>

// 虚基类
// class Poll
// {};

class Epoll /* : public Poll*/
{
    const static int gnum = 128;
    const static int gtimeout = 5000;
public:
    Epoll(int timeout = gtimeout)
        : _timeout(timeout)
    {}

    void CreateEpoll()  
    {
        // 创建epoll模型
        _epfd = epoll_create(gnum);
        if (_epfd < 0)
            exit(5);
        
    }

    // 删除epoll
    bool DelFromEpoll(int sock)
    {
        int n = epoll_ctl(_epfd, EPOLL_CTL_DEL, sock, nullptr);
        return n == 0;
    }

    // 修改Epoll状态
    bool CtrlEpoll(int sock, uint32_t events)
    {
        events |= EPOLLET;
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sock;
        int n = epoll_ctl(_epfd, EPOLL_CTL_MOD, sock, &ev);
        return n == 0;
    }

    // 添加sock到epoll
    bool addSockToEpoll(int sock, uint32_t events)
    {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sock;
        int n = epoll_ctl(_epfd, EPOLL_CTL_ADD, sock, &ev);
        return n == 0;
    }

    // 把就绪的epoll文件描述符取出来
    int WaitEpoll(struct epoll_event revs[], int num)
    {
        return epoll_wait(_epfd, revs, num, _timeout);
    }

    ~Epoll()
    {}

private:
    int _epfd;
    int _timeout;
};

// class Select : public Poll
// {};