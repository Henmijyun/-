
#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>

#include "Log.hpp"
#include "Sock.hpp"
#include "Epoll.hpp"

class TcpServer;
class Connection;

using func_t = std::function<void(Connection*)>;  // 回调函数类型

// sock要有自己独立的 接收缓冲区 && 发送缓冲区
class Connection
{
public:
    Connection(int sock = -1)
        : _sock(sock)
        , _tsvr(nullptr)
    {}

    // 设置回调
    void SetCallBack(func_t recv_cb, func_t send_cb, func_t except_cb)
    {
        _recv_cb = recv_cb;
        _send_cb = send_cb;
        _except_cb = except_cb;
    }

    ~Connection()
    {}

public:
    int _sock;   // IO文件描述符

    // 对sock进行特定的读写对应方法
    func_t _recv_cb;    // 读回调
    func_t _send_cb;    // 写回调
    func_t _except_cb;  // 异常回调
    
    // 接收缓冲区 && 发送缓冲区
    std::string _inbuffer;   // 暂时无法处理二进制流
    std::string _outbuffer;

    // 设置对TcpServer的回值指针
    TcpServer *_tsvr;
};

class TcpServer
{
    const static int gport = 8080;
    const static int gnum = 128;
public:
    TcpServer(int port = gport)
        : _port(port)
        , _revs_num(gnum)
    {
        // 1.创建listensock
        _listensock = Sock::Socket();
        Sock::Bind(_listensock, _port);
        Sock::Listen(_listensock);
        
        // 2.创建多路转接对象
        _poll.CreateEpoll();

        // 3.添加 listensock到服务器中
        AddConnection(_listensock, std::bind(&TcpServer::Accepter, this, std::placeholders::_1), nullptr, nullptr);

        // 4.构建一个获取事件的缓冲区
        _revs = new struct epoll_event[_revs_num];
    }

    // 专门针对任意sock进行添加TcpServer
    void AddConnection(int sock, func_t recv_cb, func_t send_cb, func_t except_cb)
    {
        Sock::SetNonBlock(sock);

        // 将来网络会有大量的socket，则会new大量的 Connection，需要对这些 Connection进行管理
        // 1.构建conn对象，封装sock
        Connection *conn = new Connection(sock);
        conn->SetCallBack(recv_cb, send_cb, except_cb);  // 设置回调
        conn->_tsvr = this;   // 回值指针

        // 2.添加sock[]到epoll中
        _poll.addSockToEpoll(sock, EPOLLIN | EPOLLET);  // 读+ET模式  任何多路转接服务器，一般默认只打开读取事件的关心，写入事件按需进行打开
        
        // 3.Connection*对象指针添加到_connections映射表中（用于管理）
        _connections.insert(std::make_pair(sock, conn));
    }

    // 读方法
    void Accepter(Connection *conn)
    {
        logMessage(DEBUG, "Accepter been calied");
    }

    // 一次执行，派发事件
    void LoopOne()
    {
        // 获取已经就绪的文件描述符
        int n = _poll.WaitEpoll(_revs, _revs_num);
        for (int i = 0; i < n; i ++)
        {
            int sock = _revs[i].data.fd;
            uint32_t revents = _revs[i].events;

            // 派发事件
            if (revents & EPOLLIN)  // 读事件就绪
            {
                if (IsConnectionExists(sock) && _connections[sock]->_recv_cb != nullptr)
                    _connections[sock]->_recv_cb(_connections[sock]);   // 处理读
            }
            if (revents & EPOLLOUT)  // 写事件就绪
            {
                if (IsConnectionExists(sock) && _connections[sock]->_send_cb != nullptr)
                    _connections[sock]->_send_cb(_connections[sock]);   // 处理写
            }
        }
    }

    // 根据就绪的事件，进行特定事件的派发
    void Dispather()
    {
        while (true)
        {
            LoopOne();
        }
    }

    // sock是否存在 map中
    bool IsConnectionExists(int sock)
    {
        auto iter = _connections.find(sock);  
        if (iter == _connections.end())
            return false;
        else
            return true;
    }

    ~TcpServer()
    {
        if (_listensock >= 0)
            close(_listensock);

        if (_revs)
            delete [] _revs;
    }    
private:
    int _listensock;
    int _port;
    Epoll _poll;

    // 映射关系 sock ：connection
    std::unordered_map<int, Connection *> _connections; 

    struct epoll_event *_revs;   // epoll模型缓冲区，用于存储就绪事件
    int _revs_num;               // 缓冲区大小
};