

#pragma once

#include <vector>
#include <functional>
#include <pthread.h>

#include "Sock.hpp"

namespace skk_tcpserver
{

    using func_t = std::function<void(int)>; // 回调函数
    
    class TcpServer;

    class ThreadData
    {
    public:
        ThreadData(int sock, TcpServer* server)
            :_sock(sock)
            ,_server(server)
        {}

        ~ThreadData(){}
    public:
        int _sock;
        TcpServer* _server;
    };

    class TcpServer
    {
    private:
        // 线程例程
        static void* ThreadRoutine(void* args)
        {
            pthread_detach(pthread_self());  // 线程分离
            ThreadData* td = static_cast<ThreadData*>(args);  // 强转
            td->_server->Excute(td->_sock);  // 执行任务

            close(td->_sock);
            delete td;

            return nullptr;
        }

    public:
        TcpServer(const uint16_t &port, const std::string ip = "0.0.0.0")
        {
            _listensock = _sock.Socket();      // 创建套接字
            _sock.Bind(_listensock, port, ip); // 绑定
            _sock.Listen(_listensock);         // 连接
        }

        // 设置内部方法
        void BindService(func_t func)
        {
            _func.push_back(func);
        }

        // 让线程去执行任务
        void Excute(int sock)
        {
            for (auto& f : _func)
            {
                f(sock);
            }
        }

        void Start()
        {
            for (;;)
            {
                std::string client_ip;
                uint16_t client_port;
                int sock = _sock.Accept(_listensock, &client_ip, &client_port);

                if (sock == -1)
                {
                    continue;
                }
                logMessage(NORMAL, "create new link success, sock: %d", sock);

                pthread_t tid;
                ThreadData* td = new ThreadData(sock, this);  // 用当前对象this构造
                pthread_create(&tid, nullptr, ThreadRoutine, td);  // 创建子线程
            }
        }

        ~TcpServer()
        {
            if (_listensock >= 0)
            {
                close(_listensock);
            }
        }

    private:
        int _listensock; // 监听套接字
        Sock _sock;      // 套接字
        std::vector<func_t> _func;    // 方法列表

        // std::unordered_map<std::string, func_t> _func;  // 给每一个任务起名字
    };
}