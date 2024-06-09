#ifndef __EPOLL_SERVER_HPP__
#define __EPOLL_SERVER_HPP__

#include <iostream>
#include <string>
#include <cassert>
#include <functional>

#include "Log.hpp"
#include "Sock.hpp"
#include "Epoll.hpp"

namespace ns_epoll
{
    static const int default_port = 8080;
    static const int gnum = 64;
    class EpollServer
    {
        using func_t = std::function<void(std::string)>;  // 函数类型

    public:
        EpollServer(func_t HandlerRequest, const int &port = ns_epoll::default_port)
            : _port(port)
            , _revs_num(gnum)
            , _HandlerRequest(HandlerRequest)
        {
            // 申请事件数组空间
            _revs = new struct epoll_event[_revs_num];

            // 创建listensock
            _listensock = Sock::Socket();
            Sock::Bind(_listensock, _port);
            Sock::Listen(_listensock);

            // 创建epoll模型
            _epfd = Epoll::Create();
            logMessage(DEBUG, "init success, listensock: %d, epfd: %d", _listensock, _epfd);

            // 将listensock，先添加到epoll中，让epoll帮我们管理
            if (!Epoll::CtlEpoll(_epfd, EPOLL_CTL_ADD, _listensock, EPOLLIN))
                exit(6);

            logMessage(DEBUG, "add listensock to epoll success.");
        }

        // 新链接
        void Accepter(int listensock)
        {
            std::string clientip;
            uint16_t clientport;
            int sock = Sock::Accept(listensock, &clientip, &clientport);
            if (sock < 0)
            {
                logMessage(WARNING, "accept error!");
                return;
            }

            // 将新的sock，添加到epoll（红黑树）
            if (!Epoll::CtlEpoll(_epfd, EPOLL_CTL_ADD, sock, EPOLLIN))
                return;
            logMessage(DEBUG, "add new sock : %d to epoll success.", sock);
        }

        // 读取
        void Recver(int sock)
        {
            // 1.读取数据
            char buffer[10240];
            ssize_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (n > 0)
            {
                // 假设读到完整的报文
                buffer[n] = 0;
                _HandlerRequest(buffer);  // 2.处理数据
            }
            else if (n == 0)
            {
                // 1.先在epoll去除对sock的关心
                bool res = Epoll::CtlEpoll(_epfd, EPOLL_CTL_DEL, sock, 0);
                assert(res);
                (void)res;
                // 2.再close文件
                close(sock);
                logMessage(NORMAL, "client %d quit, me too...", sock);
            }
            else
            {
                // 1.先在epoll去除对sock的关心
                bool res = Epoll::CtlEpoll(_epfd, EPOLL_CTL_DEL, sock, 0);
                assert(res);
                (void)res;
                // 2.再close文件
                close(sock);
                logMessage(NORMAL, "client recv %d error, close error sock", sock);
            }
            
        }

        // 处理事件
        void HandlerEvents(int n)
        {
            assert(n > 0);
            for (int i = 0; i < n; i++)
            {
                uint32_t revents = _revs[i].events;
                int sock = _revs[i].data.fd;
                // 读事件就绪 
                if (revents & EPOLLIN)
                {
                    if (sock == _listensock)
                    {    
                        Accepter(_listensock);  // 1. listensock 就绪
                    }
                    else
                    {    
                        Recver(sock);  // 2. 一般sock就绪 read
                    }
                }
            }
        } 

        // 获取一次就绪事件（多个）
        void LoopOnce(int timeout)
        {
            int n = Epoll::WaitEpoll(_epfd, _revs, _revs_num, timeout);
            // if (n == _revs_num) // 扩容
            switch (n)
            {
            case 0:
                logMessage(DEBUG, "timeout...");
                break;
            case -1:
                logMessage(WARNING, "epoll wait error: %s", strerror(errno));
                break;
            default:
                // 等待成功
                logMessage(DEBUG, "get a event");
                HandlerEvents(n); 
                break;
            }
        }

        void Start()
        {
            int timeout = 1000;   // 每1秒轮询  0非阻塞 -1阻塞
            while (true)
            {
                LoopOnce(timeout);
            }
        }

        ~EpollServer()
        {
            if (_listensock >= 0)
                close(_listensock);

            if (_epfd >= 0)
                close(_epfd);

            if (_revs)
                delete[] _revs;
        }

    private:
        int _listensock;           // 套接字
        int _epfd;                 // 文件描述符
        uint16_t _port;            // 端口号
        struct epoll_event *_revs; // 就绪的事件数组
        int _revs_num;             // 数组容量
        func_t _HandlerRequest;    // 回调方法
    };
}

#endif
