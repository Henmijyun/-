
#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <cerrno>
#include <vector>
#include <cassert>

#include "Log.hpp"
#include "Sock.hpp"
#include "Epoll.hpp"
#include "Protocol.hpp"

class TcpServer;
class Connection;

using func_t = std::function<void(Connection*)>;  // 回调函数类型
using callback_t = std::function<void (Connection*, std::string &request)>;  // 业务处理的回调方法

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

    // 设置对TcpServer的回指指针 （控制epoll开启）
    TcpServer *_tsvr;

    // 时间戳，用于关闭长时间不活动的链接
    uint64_t _lasttimestamp;
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
        conn->_lasttimestamp = time(); // 记录连接时间

        // 2.添加sock[]到epoll中
        _poll.addSockToEpoll(sock, EPOLLIN | EPOLLET);  // 读+ET模式  任何多路转接服务器，一般默认只打开读取事件的关心，写入事件按需进行打开
        
        // 3.Connection*对象指针添加到_connections映射表中（用于管理）
        _connections.insert(std::make_pair(sock, conn));
    }

    // 读方法
    void Accepter(Connection *conn)
    {
        // logMessage(DEBUG, "Accepter been calied");
        // 一定有listensock已经就绪，此次读取不会阻塞
        // 因为可能会有多个，所以必须循环去读
        while (true)
        {
            std::string client_ip;
            uint16_t client_port;
            int accept_errno = 0;
            int sock = Sock::Accept(conn->_sock, &client_ip, &client_port, &accept_errno);
            if (sock < 0)
            {
                // 循环读，直到读取失败
                if (accept_errno == EAGAIN || accept_errno == EWOULDBLOCK) 
                    break;
                else if (accept_errno == EINTR)
                    continue; 
                else
                {
                    // accept失败
                    logMessage(WARNING, "accept error, %d : %s", accept_errno, strerror(accept_errno));
                    break;
                } 
            }
            // 获取链接成功, 将sock托管给TcpServer
            if (sock >= 0)
            {
                AddConnection(sock, std::bind(&TcpServer::Recver, this, std::placeholders::_1),\
                                std::bind(&TcpServer::Sender, this, std::placeholders::_1),\
                                std::bind(&TcpServer::Excepter, this, std::placeholders::_1));
                logMessage(DEBUG, "accept client %s:%d success, add to epoll&&TcpServer success! sock: %d"\
                            , client_ip.c_str(), client_port, sock);
            }
            
        }
    }

    // 使能读写
    void EnableReadWrite(Connection *conn, bool readable, bool writeable)
    {
        uint32_t events = ((readable ? EPOLLIN : 0) | (writeable ? EPOLLOUT : 0));
        bool res = _poll.CtrlEpoll(_conn->_sock, events);
        assert(res);
    }

    // 从网络的数据读出来，放到输入缓冲区当中
    void Recver(Connection *conn)
    {
        conn->_lasttimestamp = time();  // 更新最近访问时间
        const num = 1024;
        bool err = false;
        // logMessage(DEBUG, "Recver event exists, Recver() been called");
        // 直接面向字节流，先进行常规读取 (非阻塞)
        while (true)
        {
            char buffer[num];
            ssize_t n = recv(conn->_sock, buffer, sizeof(buffer) - 1, 0);
            if (n < 0)
            {
                // 读取失败
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    break;  // 循环读，直到读完为止
                }
                else if (errno == EINTR)
                {
                    continue;  // 底层信号中断
                }
                else
                {
                    logMessage(ERROR, "recv error, %d : %s", errno, strerror(errno));
                    conn->_except_cb(conn); // 异常也不做处理，交给_except_cb集中处理
                    err = true;
                    break;
                }
            }
            else if (n == 0) // 关闭连接
            {
                logMessage(DEBUG, "client[%d] quit, server close [%d]", conn->_sock, conn->_sock);
                conn->_except_cb(conn);
                err = true;
                break;
            }
            else
            {
                // 读取成功
                buffer[n] = 0;
                conn->_inbuffer += buffer;  // 拼接到_inbuffer
            }
        }
        logMessage(DEBUG, "conn->_inbuffer[sock: %d] : %s", conn->_sock, conn->_inbuffer.c_str());
        if (!err)
        {
            std::vector<std::string> message;
            SpliteMessage(conn->_inbuffer, &message);  // 分割buffer
            for (auto & msg : message)
            {
                // 能保证走到这里，就是一个完整报文
                _cb(conn, msg);   // 这里可以把message封装成任务，然后push到任务队列，任务处理交给后端线程
            }
        }
    }

    // 把输出缓冲区的数据发送到网络
    void Sender(Connection *conn)
    {
        while (true)
        {
            ssize_t n = send(conn->_sock, conn->_outbuffer.c_str(), conn->_outbuffer.size(), 0);
            if (n > 0)
            {
                // 发送成功，检查是否全部发完
                conn->_outbuffer.erase(0, n);   // 移除已发数据
                if (conn->_outbuffer.empty())
                {
                    break;
                }
            }
            else
            {
                // 发送失败
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    break;  // 系统发送缓冲区满了，没发完
                }
                else if (errno == EINTR)
                {  
                    continue;  // 信号中断
                }
                else
                {
                    logMessage(ERROR, "recv error, %d : %s", errno, strerror(errno));
                    conn->_except_cb(conn); // 异常也不做处理，交给_except_cb集中处理
                    break;
                }
            }
        }
        // 有可能没发完，但是可以让它统一处理（发完/发送条件不满足）
        if (conn->_outbuffer.empty())
        {
            EnableReadWrite(conn, true, false);  // 发完，关掉 “写”
        }
        else
        {
            EnableReadWrite(conn, true, true);  // 没发完，继续关心 “写”
        }
    }

    // 读写异常/链接异常 处理
    void Excepter(Connection *conn)
    {
        if (IsConnectionExists(conn->_sock))
        {
            return; // 不存在
        }
        else
        {
            // 1. 从epoll中移除
            bool ret = _poll.DelFromEpoll(conn->_sock); 
            assert(ret);
            
            // 2. 把unorder_map中的文件描述符移除
            _connections.erase(coonn->_sock);

            // 3. close(sock);
            close(conn->_sock);

            // 4. delete conn
            delete conn;

            logMessage(DEBUG, "Excepter 回收完毕所有的异常情况");
        }
    }

    // 一次执行，派发事件
    void LoopOnce()
    {
        // 获取已经就绪的文件描述符
        int n = _poll.WaitEpoll(_revs, _revs_num);
        for (int i = 0; i < n; i ++)
        {
            int sock = _revs[i].data.fd;
            uint32_t revents = _revs[i].events;

            // 所有事件，都设置读写就绪 （所有事件（包括异常）都交给read/write来统一处理）
            if (revents & EPOLLERR)  // 异常报错
            {
                revents |= (EPOLLIN | EPOLLOUT);
            } 
            if (revents & EPOLLHUP)  // 文件描述符被挂断
            {
                revents |= (EPOLLIN | EPOLLOUT);
            }

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

    void ConnectAliveCheck()
    {
        // 遍历所有的_connections，通过检查conn最近活动的时间，如果长时间不活动，就进入链接超时的逻辑
        for (auto &iter : _connections)
        {
            uint64_t currtime = time();
            uint64_t deadtime  = currtime - (iter->_lasttimestamp);
            if (deadtime > /*超时时间*/)
            {
                // 差错处理
            }
        }
    }

    // 根据就绪的事件，进行特定事件的派发
    void Dispather(callback_t cb)
    {
        _cb = cb;  // 事件派发前，先设置事件回调
        while (true)
        {
            // ConnectAliveCheck();  // 检查是否超时
            LoopOnce();         // 把epoll当定时器使用
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
    // 网络服务器-------
    int _listensock;
    int _port;
    Epoll _poll;
    std::unordered_map<int, Connection *> _connections; // 映射关系 sock ：connection
    struct epoll_event *_revs;   // epoll模型缓冲区，用于存储就绪事件
    int _revs_num;               // 缓冲区大小

    // 上层业务处理--------
    callback_t _cb;

};