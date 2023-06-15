#ifndef __UDP_SERVER_HPP__
#define __UDP_SERVER_HPP__

#include "log.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

class UdpServer
{
public:
    UdpServer(uint16_t port, std::string ip = "0.0.0.0")
        :_port(port)
        ,_ip(ip)
        ,_sock(-1)
    {}

    bool initServer()
    {
        // 从这里开始，就是新的系统调用，来完成网络功能
        // 1.创建套接字
        _sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sock < 0)
        {
            logMessage(FATAL, "%d:%s", errno, strerror(errno));
            exit(2);
        }

        // 2.bind：将用户设置的ip和port在内核中和当前进程强关联
        
    }
    void Start()
    {}

    ~UdpServer()
    {}
private:
    // 一个服务器，一般必须需要IP地址和port(16位的整数)
    uint16_t _port;
    std::string _ip;
    int _sock;         // 套接字 (文件描述符)

};

#endif