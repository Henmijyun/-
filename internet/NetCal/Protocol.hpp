// 定制协议 （自定义）

#pragma once

#include <iostream>
#include <cstring>


namespace skk_protocol
{
#define MYSELF 1
#define SPACE " "
#define SPACE_LEN strlen(SPACE)

    // 请求
    class Request
    {
    public:
        // 1.自主实现 
        // 2.使用现成的方案

        // 序列化   数据转字符串 "_x _op _y"
        std::string Serialize()
        {
#ifdef MYSELF
            std::string str;
            str = std::to_string(_x);
            str += SPACE;
            str += _op;
            str += SPACE;
            str += std::to_string(_y);
            return str;
#else
            std::cout << "TODO" << std::endl;
#endif
        }

        // 反序列化  "_x _op _y"字符串转数据
        bool Deserialized(const std::string& str)
        {
#ifdef MYSELF
            std::size_t left = str.find(SPACE);  // 左边是否有空格
            if (left == std::string::npos)
            {
                return false;
            }
            std::size_t right = str.rfind(SPACE);  // 右边是否有空格
            if (right == std::string::npos)
            {
                return false;
            }

            _x = atoi(str.substr(0, left).c_str());   // 取第一个数据转整型
            _y = atoi(str.substr(right + SPACE_LEN).c_str());  // 取第二个数据
            if (left + SPACE_LEN > str.size())
            {
                return false;
            }
            else
            {
                _op = str[left + SPACE_LEN];  // 取符号
            }
            return true;
#else
            std::cout << "TODO" << std::endl;
#endif        
        }

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

    // 回复
    class Response
    {
    public:
        // 序列化  数据转字符串"_code _result"
        std::string Serialize()
        {
#ifdef MYSELF
            std::string str;
            str = std::to_string(_code);
            str += SPACE;
            str += std::to_string(_result);
            return str;
#else
            std::cout << "TODO" << std::endl;
#endif              
        }

        // 反序列化   "_code _result"字符串转数据
        bool Deserialized(const std::string& str)
        {
#ifdef MYSELF
            std::size_t pos = str.find(SPACE);
            if (pos == std::string::npos)
            {
                return false;
            }
            _code = atoi(str.substr(0, pos).c_str());   // 取code数据转整型
            if (pos + SPACE_LEN > str.size())
            {
                return false;
            }
            _result = atoi(str.substr(pos + SPACE_LEN).c_str());  // 取result数据转整型

            return true;
#else
            std::cout << "TODO" << std::endl;
#endif                          
        }

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

    // 读请求  (临时方案)
    std::string Recv(int sock)
    {
        char inbuffer[1024];
        ssize_t s = recv(sock, inbuffer, sizeof(inbuffer), 0);
        if (s > 0)
        {
            return inbuffer;
        }
    }

    // 发送数据
    void Send(int sock, const std::string str)
    {
        send(sock, str.c_str(), str.size(), 0);
    }


}



