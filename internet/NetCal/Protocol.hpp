// 定制协议 （自定义）

#pragma once

#include <iostream>



namespace skk_protocol
{
    class Request
    {
    public:
        // 序列化
        std::string Serialize()
        {}

        // 反序列化
        bool Deserialized(const std::string& package)
        {}

    public:
        Request()
        {}

        Request(int x, int y, char op)
            :_x(x)
            ,_y(y)
            ,_op(op)
        {}

        Request(const Request& copy)
        {
            _x = copy._x;
            _y = copy._y;
            _op = copy._op;
        }

        ~Request(){}
        
    public:
        int _x;
        int _y;
        char _op;  // '+' '-' '*' '/' '%'  
    };

    // 请求对象
    class Response
    {
    public:
        // 序列化
        std::string Serialize()
        {}

        // 反序列化
        bool Deserialized(const std::string& package)
        {}

    public: 
        Response()
        {}
        
        Response(int result, int code)
            :_result(result)
            ,_code(code)
        {}

        ~Response(){}
    public:
        int _result;  // 计算结果 
        int _code;    // 计算结果的状态码
    };

    // 读写方法  (临时方案)
    std::string Recv(int sock)
    {
        char inbuffer[1024];
        ssize_t s = recv(sock, inbuffer, sizeof(inbuffer), 0);
        if (s > 0)
        {
            return inbuffer;
        }
    }
}



