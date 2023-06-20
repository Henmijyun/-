
#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "log.hpp"

// 利用服务套接字进行通信
static void service(int sock, const std::string& client_ip, const uint16_t& client_port)
{
    // echo server
    char buffer[1024];
    while (true)
    {
        // read && write 可以直接被使用
        ssize_t s = read(sock, buffer, sizeof(buffer)-1);
        if (s > 0)  // 读取成功
        {
            buffer[s] = 0;  //将发过来的数据当做字符串
            std::cout << client_ip << ":" << client_port << "# " << buffer << std::endl;
        }
        else if(s == 0)   // 对端关闭连接
        {
            logMessage(NORMAL, "%s:%d shutdown, me too!", client_ip.c_str(), client_port);
            break;
        }
        else   // 读取套接字失败
        {
            logMessage(ERROR, "read socket error, %d:%s", errno, strerror(errno));
            break;
        }

        // 读取成功, 发回去
        write(sock, buffer, strlen(buffer));

    }
}

class TcpServer
{
public:
    const static int gbacklog = 20;  

public:
    TcpServer(uint16_t port, std::string ip = "")
        :_port(port)
        ,_ip(ip)
        ,_listensock(-1)
    {}
    
    void initServer()   // 初始化
    {
        // 1. 创建sock  -- 进程和文件
        _listensock = socket(AF_INET, SOCK_STREAM, 0);   // 创建套接字
        if (_listensock < 0)
        {
            logMessage(FATAL, "create socket error, %d:%s", errno, strerror(errno));
            exit(2);
        }

        logMessage(NORMAL, "create socket success, _listensock: %d", _listensock);   // 查看套接字  默认3

        // 2. bind  -- 文件+网络
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;      // 填充第一个字段为AF_INET
        local.sin_port = htons(_port);   // 主机序列 转 网络序列
        local.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());  // 点分十进制字符串IP地址 -> 4字节主机序列 -> 网络序列
        
        if (bind(_listensock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            // bind失败
            logMessage(FATAL, "bind error, %d:%s", errno, strerror(errno));
            exit(3);
        }

        // 3. TCP是面向链接的，所以通信前需要先建立链接
        if (listen(_listensock, gbacklog) < 0) 
        {
            // 切换监听状态失败
            logMessage(FATAL, "listen error, %d:%s", errno, strerror(errno));
            exit(4);
        }

        // 初始化完成
        logMessage(NORMAL, "init server success");

    }

    void Start()  // 运行
    {
        signal(SIGCHLD, SIG_IGN);  // 对SIGCHLD，主动忽略SIGCHLD信号，子进程退出的时候，会自动释放自己的僵尸状态
        while (true)
        {
            // sleep(1);
            // 4. 获取链接
            struct sockaddr_in src;
            socklen_t len = sizeof(src);
            // servicesock(李四、王五.. -- 提供服务) vs _sock(张三 -- 获取新连接)
            int servicesock = accept(_listensock, (struct sockaddr*)&src, &len);  // 获取链接（获取服务套接字）
            if (servicesock < 0)
            {
                logMessage(ERROR, "accept error, %d:%s", errno, strerror(errno));
                continue;
            }

            // 获取连接成功
            uint16_t client_port = ntohs(src.sin_port);       // 网络序列->主机序列
            std::string client_ip = inet_ntoa(src.sin_addr);  // 网络序列->点分十进制字符串  
            logMessage(NORMAL, "link success, \
                servicesock: %d | %s : %d |\n", 
                servicesock, client_ip.c_str(), client_port);
            
            // 开始通信服务
            // version 1.0 -- 单进程循环版 -- 一次只能处理一个客户端，处理完一个之后，才能处理下一个 
            // 因为单进程循环读取，另一个在阻塞等待
            // service(servicesock, client_ip, client_port);

            // version 2.0 -- 多进程版 -- 创建子进程，让子进程给新连接提供服务
            // 子进程可以打开父进程的 文件描述符fd
            pid_t id = fork();
            assert(id != -1);
            if (id == 0)
            {
                // 子进程，子进程会继承父进程打开的文件和fd
                // 子进程只提供服务，不需要监听socket
                close(_listensock);
                service(servicesock, client_ip, client_port);

                exit(0);   // 主动忽略SIGCHLD信号, 僵尸状态自动退出
            }
            
            // 父进程关闭被子进程继承了的文件描述符
            close(servicesock);
            
            // waitpid();   // 进程等待


            
        }
    }

    ~TcpServer()
    {}

private:
    uint16_t _port;     // 端口
    std::string _ip;    // IP地址
    int _listensock;    // 监听套接字
};



#endif