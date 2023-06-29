// 线程池版 -- 线程池向每个线程派发服务

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

#include "ThreadPool/log.hpp"
#include "ThreadPool/threadPool.hpp"
#include "ThreadPool/Task.hpp"



// 利用服务套接字进行通信
static void service(int sock, const std::string& client_ip,\
        const uint16_t& client_port, const std::string& thread_name)
{
    // echo server
    // 长连接：从连上到断开，一直保持着链接，就算不通信 （资源浪费）
    // 优化方案（定期关闭链接）
    char buffer[1024];
    while (true)
    {
        // read && write 可以直接被使用
        ssize_t s = read(sock, buffer, sizeof(buffer)-1);
        if (s > 0)  // 读取成功
        {
            buffer[s] = 0;  //将发过来的数据当做字符串
            std::cout << thread_name << "|" << client_ip << ":" << client_port << "# " << buffer << std::endl;
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
    close(sock);
}

// 小写转大写的服务
static void change(int sock, const std::string& client_ip,\
        const uint16_t& client_port, const std::string& thread_name)
{
    // echo server
    char buffer[1024];

    // read && write 可以直接被使用
    ssize_t s = read(sock, buffer, sizeof(buffer)-1);
    if (s > 0)  // 读取成功
    {
        buffer[s] = 0;  //将发过来的数据当做字符串
        std::cout << thread_name << "|" << client_ip << ":" << client_port << "# " << buffer << std::endl;
        
        // 转大写服务  a -> A
        std::string message;
        char* start = buffer;
        while (*start)
        {
            char c;
            if (islower(*start)) 
            {
                c = toupper(*start);
            }
            else
            {
                c = *start;
            }
            message.push_back(c);
            start++;
        }

        // 读取成功, 发回去
        write(sock, message.c_str(), message.size());
    }
    else if(s == 0)   // 对端关闭连接
    {
        logMessage(NORMAL, "%s:%d shutdown, me too!", client_ip.c_str(), client_port);
    }
    else   // 读取套接字失败
    {
        logMessage(ERROR, "read socket error, %d:%s", errno, strerror(errno));
    }

    close(sock);
}

// 英汉互译的服务
static void dictOnline(int sock, const std::string& client_ip,\
        const uint16_t& client_port, const std::string& thread_name)
{
    // echo server
    char buffer[1024];
    static std::unordered_map<std::string, std::string> dict = {
        {"apple", "苹果"},
        {"banana", "香蕉"},
        {"hard", "好难"}
    };

    // read && write 可以直接被使用
    ssize_t s = read(sock, buffer, sizeof(buffer)-1);
    if (s > 0)  // 读取成功
    {
        buffer[s] = 0;  //将发过来的数据当做字符串
        std::cout << thread_name << "|" << client_ip << ":" << client_port << "# " << buffer << std::endl;
        
        // 查找
        std::string message;
        auto iter = dict.find(buffer);
        if (iter == dict.end())
        {
            message = "我不知道..";
        }
        else
        {
            message = iter->second;
        }

        // 读取成功, 发回去
        write(sock, message.c_str(), message.size());
    }
    else if(s == 0)   // 对端关闭连接
    {
        logMessage(NORMAL, "%s:%d shutdown, me too!", client_ip.c_str(), client_port);
    }
    else   // 读取套接字失败
    {
        logMessage(ERROR, "read socket error, %d:%s", errno, strerror(errno));
    }

    close(sock);
}

class TcpServer
{
public:
    const static int gbacklog = 20; 

public:
    TcpServer(uint16_t port, std::string ip = "0.0.0.0")
        :_port(port)
        ,_ip(ip)
        ,_listensock(-1)
        ,_threadpool_ptr(ThreadPool<Task>::getThreadPool())
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
        inet_pton(AF_INET, _ip.c_str(), &local.sin_addr);  // 同下
        // inet_aton(_ip.c_str(), &local.sin_addr);  // 同下
        // local.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());  // 点分十进制字符串IP地址 -> 4字节主机序列 -> 网络序列
        
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
        _threadpool_ptr->run(); 
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
            logMessage(NORMAL, "link success, servicesock: %d | %s : %d |\n",\
                servicesock, client_ip.c_str(), client_port);
            
            // 开始通信服务
            // version 1.0 -- 单进程循环版 -- 一次只能处理一个客户端，处理完一个之后，才能处理下一个 
            // version 2.0 -- signal方式的多进程版 -- 创建子进程，让子进程给新连接提供服务
            // version 2.1 -- 多进程版 -- 子进程负责生孙子，孙子进程负责服务
            // version 3.0 -- 多线程版 -- 线程分离，每个线程独立服务

            // version 4.0 -- 线程池版 -- 线程池向每个线程派发服务
            // Task t(servicesock, client_ip, client_port, service);  // 通信服务
            // Task t(servicesock, client_ip, client_port, change);   // 大小写转换服务
            Task t(servicesock, client_ip, client_port, dictOnline);   // 英汉转换服务
            _threadpool_ptr->pushTask(t);   // 任务插入到线程队列

        }
    }

    ~TcpServer()
    {}

private:
    uint16_t _port;     // 端口
    std::string _ip;    // IP地址
    int _listensock;    // 监听套接字
    std::unique_ptr<ThreadPool<Task>> _threadpool_ptr;   // 线程池
};



#endif