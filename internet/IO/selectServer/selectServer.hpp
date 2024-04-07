#ifndef __SELECT_SVR_H__
#define __SELECT_SVR_H__

#include <iostream>
#include <string>
#include <vector>

#include <sys/select.h>
#include <sys/time.h>

#include "Sock.hpp"
#include "Log.hpp"

#define BITS 8
#define NUM (sizeof(fd_set) * BITS)
#define FD_NONE -1

using namespace std;

// select 只完成读取，写入和异常不做处理 -- （后面写epoll的时候再补全）
class SelectServer
{
public:
    SelectServer(const uint16_t &port = 8080)
        : _port(port)
    {
        _listensock = Sock::Socket();   // 创建套接字
        Sock::Bind(_listensock, _port); // 绑定
        Sock::Listen(_listensock);      // 监听
        logMessage(DEBUG, "%s", "create base socket success");
        for (int i = 0; i < NUM; i++)
        {
            _fd_array[i] = FD_NONE; // 文件描述符初始化（只有大于等于0才是被使用的）
        }

        // 约定_fd_array[0] = listensock;
        _fd_array[0] = _listensock;
    }

    void Start()
    {

        while (true)
        {
            DebugPrint();
            // 文件描述符集
            fd_set rfds;
            FD_ZERO(&rfds); // 清空文件描述符集
            int maxfd = _listensock;

            // 将_listensock添加到读文件描述符集
            // FD_SET(_listensock, &rfds);

            for (int i = 0; i < NUM; i++)
            {
                if (_fd_array[i] == FD_NONE) // 检查该文件描述符是否有在用
                {
                    continue;
                }
                FD_SET(_fd_array[i], &rfds); // 添加到文件描述符集
                if (maxfd < _fd_array[i])
                {
                    maxfd = _fd_array[i]; // 更新最大的文件描述符
                }
            }

            struct timeval timeout = {0, 0}; // 每 3秒，0毫秒 查一次

            // rfds会有两类套接字sock，一类是listensock，一类是普通sock，都需要处理
            int n = select(maxfd + 1, &rfds, nullptr, nullptr, nullptr);
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
                logMessage(DEBUG, "get a new link event..."); // 一直循环打印的原因：如果连接建立完成，就绪了，没有把数据取走，就会一直通知

                HandlerEvent(rfds);
                break;
            }
        }
    }

    ~SelectServer()
    {
        if (_listensock >= 0)
        {
            close(_listensock); // 关闭文件描述符
        }
    }

private:
    // 处理就绪的事件
    void HandlerEvent(const fd_set &rfds) // fd_set是一个集合，里面有多个sock
    {
        // 区分文件描述符
        for (int i = 0; i < NUM; i++)
        {
            // 1.去掉不合法的fd
            if (_fd_array[i] == FD_NONE)
            {
                continue;
            }
            // 2.合法的fd，判断是否就绪
            if (FD_ISSET(_fd_array[i], &rfds))
            {
                // 指定的fd，读事件就绪
                if (i == 0 && _fd_array[i] == _listensock)
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
        // 2.不能直接读，因为不清楚sock上面实际数据什么时候到来（现在只是握手成功）
        // 这里进行recv,read，就会先被阻塞！
        // 应该让新的sock托管给select（添加到_fd_array数组中），让select帮我们进行检测sock上是否有新的数据就绪。
        // 有数据就绪->select读事件就绪->就会通知->再进行读取，实现不阻塞
        int pos = 1;
        for (; pos < NUM; pos++)
        {
            if (_fd_array[pos] == FD_NONE) // 查找没被使用的数组位置
            {
                break;
            }
        }

        if (pos == NUM) // 数组满了
        {
            logMessage(WARNING, "select server already full, close: %d", sock);
            close(sock);
        }
        else
        {
            _fd_array[pos] = sock; // 把文件描述符添加到数组
        }
    }

    // 读事件就绪，读数据
    void Recver(int pos)
    {
        // 读事件就绪：INPUT事件到来,recv、read
        logMessage(DEBUG, "message in, get IO event: %d", _fd_array[pos]);
        // select 已经进行了事件检测，fd上的数据一定就绪，不会阻塞。
        // TCP面向字节流->所以下面代码无法保证读到一个完整的报文！
        char buffer[1024];
        int n = recv(_fd_array[pos], buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            logMessage(DEBUG, "client[%d]# %s", _fd_array[pos], buffer);
        }
        else if (n == 0) // 退出
        {
            logMessage(DEBUG, "client[%d] quit, me too...", _fd_array[pos]);
            // 1.关闭不需要的fd
            close(_fd_array[pos]);
            // 2.让select不要关心对应的fd
            _fd_array[pos] = FD_NONE;
        }
        else
        {
            // 读取出错
            logMessage(WARNING, "%d sock recv error, %d : %s", _fd_array[pos], errno, strerror(errno));
            // 1.关闭不需要的fd
            close(_fd_array[pos]);
            // 2.让select不要关心对应的fd
            _fd_array[pos] = FD_NONE;
        }
    }


    // test
    void DebugPrint()
    {
        std::cout << "_fd_array[]: ";
        for (int i = 0; i < NUM; i++)
        {
            if (_fd_array[i] == FD_NONE)
            {
                continue;
            }
            std::cout << _fd_array[i] << " ";
        }

        std::cout << std::endl;
    }

private:
    // 如果用C写，就用全局变量 
    uint16_t _port;  // 端口号
    int _listensock; // 套接字

    // std::vector<int> _fd_array;
    int _fd_array[NUM]; // 保存文件描述符
    
};

#endif