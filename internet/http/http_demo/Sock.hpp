// 套接字对象

#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <memory>
#include<unordered_map>

#include <ctype.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Log.hpp"


class Sock
{
public:
    const static int gbacklog = 20; 

public:
    Sock()
    {}
    
    // 创建套接字sock  -- 进程和文件
    int Socket()
    {
        int listensock = socket(AF_INET, SOCK_STREAM, 0);   // 创建套接字
        if (listensock < 0)
        {
            logMessage(FATAL, "create socket error, %d:%s", errno, strerror(errno));
            exit(2);
        }

        logMessage(NORMAL, "create socket success, _listensock: %d", listensock);   // 查看套接字  默认3

        // 地址复用，让对应的port、ip 在TIME_WAIT状态时，也可以立马重新启动
        int opt = 1;
        setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        return listensock;
    }

    // 绑定bind  -- 文件+网络
    void Bind(int sock, uint16_t port, std::string ip = "0.0.0.0")
    {    
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;      // 填充第一个字段为AF_INET
        local.sin_port = htons(port);   // 主机序列 转 网络序列
        inet_pton(AF_INET, ip.c_str(), &local.sin_addr);  // 点分十进制字符串IP地址 -> 4字节主机序列 -> 网络序列

        if (bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            // bind失败
            logMessage(FATAL, "bind error, %d:%s", errno, strerror(errno));
            exit(3);
        }
    }

    // 连接，TCP是面向链接的，所以通信前需要先建立链接
    void Listen(int sock)
    {

        if (listen(sock, gbacklog) < 0) 
        {
            // 切换监听状态失败
            logMessage(FATAL, "listen error, %d:%s", errno, strerror(errno));
            exit(4);
        }

        // 初始化完成
        logMessage(NORMAL, "init server success");
    }

    // 获取链接
    // const std::string&  输入型参数
    // std::string*   输出型参数
    // std::string&   输入输出型参数
    int Accept(int listensock, std::string* ip, uint16_t* port)
    {
        struct sockaddr_in src;
        socklen_t len = sizeof(src);
        int servicesock = accept(listensock, (struct sockaddr*)&src, &len);  // 获取链接（获取服务套接字）
        if (servicesock < 0)
        {
            logMessage(ERROR, "accept error, %d:%s", errno, strerror(errno));
            return -1;
        }

        // 获取连接成功
        if (port)
        {
            *port = ntohs(src.sin_port);       // 网络序列->主机序列
        }
        if (ip)
        {
            *ip = inet_ntoa(src.sin_addr);  // 网络序列->点分十进制字符串  
        }

        return servicesock;
    }

    // 连接
    bool Connect(int sock, const std::string& server_ip, const uint16_t& server_port)
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(server_port);
        server.sin_addr.s_addr = inet_addr(server_ip.c_str());

        // 连接
        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    ~Sock()
    {}

};



#endif