
#pragma once

#include <iostream>
#include <functional>
#include <signal.h>

#include "Log.hpp"
#include "Sock.hpp"


class HttpServer
{
public:
    using func_t = std::function<void(int)>;

public:
    HttpServer(const uint16_t& port, func_t func)
        :_port(port)
        ,_func(func)
    {
        _listensock = _sock.Socket();  // 创建套接字
        _sock.Bind(_listensock, _port);  // 绑定
        _sock.Listen(_listensock);  // 连接
    }

    void Start()
    {
        signal(SIGCHLD, SIG_IGN);   // 信号忽略
        for ( ; ; )
        {
            std::string client_ip;
            uint16_t client_port;
            int sockfd = _sock.Accept(_listensock, &client_ip, &client_port);
            if (sockfd < 0)
            {
                continue;
            }
            if (fork() == 0)
            {
                // 子进程
                close(_listensock);
                _func(sockfd);
                close(sockfd);
                exit(0);
            }
            close(sockfd);
        }
    }

    ~HttpServer()
    {
        if (_listensock >= 0) 
        {
            close(_listensock);
        }
    }
private:
    int _listensock;
    uint16_t _port;
    Sock _sock;
    func_t _func;
};