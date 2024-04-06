#ifndef __SELECT_SVR_H__
#define __SELECT_SVR_H__

#include <iostream>

#include <sys/select.h>
#include <time.h>

#include "Sock.hpp"
#include "Log.hpp"

using namespace std;

// select 只完成读取，写入和异常不做处理 -- （后面写epoll的时候再补全）
class SelectServer
{
public:
    SelectServer(const uint16_t &port = 8080)
        :_port(port)
    {
        _listensock = Sock::Socket();    // 创建套接字
        Sock::Bind(_listensock, _port);  // 绑定
        Sock::Listen(_listensock);      // 监听
        logMessage(DEBUG, "%s","create base socket success");
    }

    void Start()
    {
        // 文件描述符集
        fd_set rfds;
        FD_ZERO(&rfds);  // 清空
        while (true)
        {
            struct timeval timeout = {3, 0};  // 3秒，0毫秒

            // 将_listensock添加到读文件描述符集
            FD_SET(_listensock, &rfds);  
            int n = select(_listensock + 1, &rfds, nullptr, nullptr, &timeout);
            switch (n)
            {
            case 0:
                logMessage(DEBUG, "%s", "time out..");  // 超时
                // 还没就绪，可以先返回做其他事
                break;
            case -1:
                logMessage(WARNING, "select errer: %d : %s", errno, strerror(errno));  //失败
                break;
            default:
                // 链接成功
                logMessage(DEBUG, "get a new link event...");   // 一直循环打印的原因：如果连接建立完成，就绪了，没有把数据取走，就会一直通知
                
                HandlerEvent(rfds);
                break;  
            }
        }
    }

    ~SelectServer()
    {
        if (_listensock >= 0)
        {
            close(_listensock);  // 关闭文件描述符
        }
    }

private:
    // 处理就绪的事件
    void HandlerEvent(const fd_set& rfds)  // fd_set是一个集合，里面有多个sock
    {
        string clientip;
        uint16_t clientport = 0;
        if (FD_ISSET(_listensock, &rfds))  // 判断是否在集合里
        {
            // 时间就绪，可以进行读取
            // 获取新连接
            int sock = Sock::Accept(_listensock, &clientip, &clientport);  // 在这里Accept不会阻塞
            if (sock < 0)
            {
                logMessage(WARNING, "accept error");
                return;
            }
            
            logMessage(DEBUG, "get a new line success : [%s:%d] : %d", clientip.c_str(), clientport, sock);
            // TODO

        }   
    }


private:
    uint16_t _port;   // 端口号
    int _listensock;   // 套接字



};

#endif