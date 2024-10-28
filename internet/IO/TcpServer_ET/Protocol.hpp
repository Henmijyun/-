
#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <vector>

// 1.报文与报文之间，用特殊字符解决粘包问题
// 2.获取一个一个独立完整的报文，序列和反序列 -- 自定义
// 例：10+2X10+2X10+2X10+2

// 支持解决粘包问题，处理独立报文
#define SEP "X"
#define SEP_LEN strlen(SEP)

// 自定义手写序列、反序列化
#define SPACE " "
#define SPACE_LEN strlen(SPACE)


// 把传入的缓冲区进行切分
// 1. buffer被切走的部分，也要从buffer中移除
// 2. 可能会存在多个报文，多个报文依次放入out
// buffer ： 输入型参数
// out ： 输出型参数
void SpliteMessage(std::string &buffer, std::vector<std::string> *out)
{
    // 例：10+2X10+2X10+2X10+2
    while (true)
    {
        auto pos = buffer.find(SEP);
        if (std::string::npos == pos)
        {
            break; // 没找到
        }
        std::string message = buffer.substr(0, pos); // 获取
        buffer.erase(0, pos + SEP_LEN);  // 移除
        out->push_back(message); // 插入
        
        // std::cout << "debug: " << message << " : " << buffer << std::endl;
        // sleep(1); 
    }
}

// 应答的
std::string Encode(std::string& s)
{
    return s + SEP;
}

// 定制协议 （自定义）

// 请求  client -> server
class Request
{
public:
    // 1.自主实现   "length\r\n_x _op _y\r\n"
    // 2.使用现成的方案  json

    // 序列化   数据转字符串 "_x _op _y"
    std::string Serialize()
    {
        std::string str;
        str = std::to_string(_x);
        str += SPACE;
        str += _op;
        str += SPACE;
        str += std::to_string(_y);
        return str;
    }

    // 反序列化  "_x _op _y"字符串转数据
    bool Deserialized(const std::string& str)
    {
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



// 响应 server -> client
class Response
{
public:
    // 序列化  数据转字符串"_code _result"
    std::string Serialize()
    {
        std::string str;
        str = std::to_string(_code);
        str += SPACE;
        str += std::to_string(_result);
        return str;            
    }

    // 反序列化   "_code _result"字符串转数据
    bool Deserialized(const std::string& str)
    {
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
    int _code;    // 计算结果的状态码  0正确
};

