#ifndef __POLL_SVR_H__
#define __POLL_SVR_H__

#include <iostream>
#include <string>
#include <vector>

#include <poll.h>
#include <sys/time.h>

#include "Sock.hpp"
#include "Log.hpp"


#define FD_NONE -1

using namespace std;

class PollServer
{
public:
    static const int nfds = 100;
public:
    PollServer(const uint16_t &port = 8080)
        : _port(port)
        , _nfds(nfds)
    {
        _listensock = Sock::Socket();   // 创建套接字
        Sock::Bind(_listensock, _port); // 绑定
        Sock::Listen(_listensock);      // 监听
        logMessage(DEBUG, "%s", "create base socket success");

        _fds = new struct pollfd[_nfds];
        for (int i = 0; i < _nfds; i++)
        {
            _fds[i].fd = FD_NONE;
            _fds[i].events = _fds[i].revents = 0;
        }
        _fds[0].fd = _listensock;
        _fds[0].events = POLLIN;  //位图   读 POLLIN | POLLOUT  写

        _timeout = 1000;
    }

    void Start()
    {

        while (true)
        {
            int n = poll(_fds, _nfds, _timeout);
            switch (n)
            {
            case 0:
                logMessage(DEBUG, "%s", "time out.."); // 超时
                // 还没就绪，可以先返回做其他事
                break;
            case -1:
                logMessage(WARNING, "select errer: %d : %s", errno, strerror(errno)); // 失败
                break;
            default:
                // 链接成功
                HandlerEvent();
                break;
            }
        }
    }

    ~PollServer()
    {
        if (_listensock >= 0)
        {
            close(_listensock); // 关闭文件描述符
        }
        if (_fds)
        {
            delete [] _fds;
        }
    }

private:
    // 处理就绪的事件
    void HandlerEvent() // fd_set是一个集合，里面有多个sock
    {
        // 区分文件描述符
        for (int i = 0; i < _nfds; i++)
        {
            // 1.去掉不合法的fd
            if (_fds[i].fd == FD_NONE)
            {
                continue;
            }
            // 2.合法的fd，判断是否就绪
            if (_fds[i].revents & POLLIN)
            {
                // 指定的fd，读事件就绪
                if (_fds[i].fd == _listensock)
                {
                    // 读时间就绪：连接事件到来
                    Accepter(); // 获取新连接
                }
                else
                {
                    // 读事件就绪，读数据
                    Recver(i);
                }
            }
        }
    }


    // 获取新连接
    void Accepter()
    {
        string clientip;
        uint16_t clientport = 0;

        // 0.连接事件就绪（三次握手），可以进行读取
        // 1.获取新连接
        int sock = Sock::Accept(_listensock, &clientip, &clientport); // 在这里Accept不会阻塞
        if (sock < 0)
        {
            logMessage(WARNING, "accept error");
            return;
        }

        logMessage(DEBUG, "get a new link success : [%s:%d] : %d", clientip.c_str(), clientport, sock);

        int pos = 1;
        for (; pos < _nfds; pos++)
        {
            if (_fds[pos].fd == FD_NONE) // 查找没被使用的数组位置
            {
                break;
            }
        }

        if (pos == _nfds) // 数组满了
        {
            // 这里可以设置让struct pollfd自动扩容
            logMessage(WARNING, "select server already full, close: %d", sock);
            close(sock);
        }
        else
        {
            _fds[pos].fd = sock; // 把文件描述符添加到数组
            _fds[pos].events = POLLIN;
        }
    }

    // 读事件就绪，读数据
    void Recver(int pos)
    {
        // 读事件就绪：INPUT事件到来,recv、read
        logMessage(DEBUG, "message in, get IO event: %d", _fds[pos]);

        char buffer[1024];
        int n = recv(_fds[pos].fd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            logMessage(DEBUG, "client[%d]# %s", _fds[pos].fd, buffer);
        }
        else if (n == 0) // 退出
        {
            logMessage(DEBUG, "client[%d] quit, me too...", _fds[pos].fd);
            // 1.关闭不需要的fd
            close(_fds[pos].fd);
            // 2.让poll不要关心对应的fd
            _fds[pos].fd = FD_NONE;
            _fds[pos].events = 0;
        }
        else
        {
            // 读取出错
            logMessage(WARNING, "%d sock recv error, %d : %s", _fds[pos].fd, errno, strerror(errno));
            close(_fds[pos].fd);
            // 2.让poll不要关心对应的fd
            _fds[pos].fd = FD_NONE;
            _fds[pos].events = 0;
        }
    }


    // test
    void DebugPrint()
    {
        std::cout << "_fd_array[]: ";
        for (int i = 0; i < _nfds; i++)
        {
            if (_fds[i].fd == FD_NONE)
            {
                continue;
            }
            std::cout << _fds[i].fd << " ";
        }

        std::cout << std::endl;
    }

private:
    uint16_t _port;   // 端口号
    int _listensock;  // 套接字
    struct pollfd *_fds;   // poll结构体
    int _nfds;      // nfds_t  文件描述符个数
    int _timeout;   // 时间

    
};

#endif