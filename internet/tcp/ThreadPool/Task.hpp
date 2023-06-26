#include <iostream>
#include <functional>
#include "log.hpp"

// typedef std::function<void(int, const std::string&, const uint16_t&)> func_t;
using func_t = std::function<void(int, const std::string&, \
                    const uint16_t&, const std::string&)>;

class Task
{
public:
    Task()
    {}
    
    Task(int sock, const std::string ip, uint16_t port, func_t func)
        :_sock(sock)
        ,_ip(ip)
        ,_port(port)
        ,_func(func)
    {}

    void operator()(const std::string& name)
    {
        _func(_sock, _ip, _port, name);
    }
    
public:
    int _sock;           // 套接字
    std::string _ip;     // IP地址
    uint16_t _port;      // 端口
    func_t _func;        // 回调函数
};