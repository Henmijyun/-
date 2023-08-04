// 定制协议 （自定义）

#pragma once

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>

#include <jsoncpp/json/json.h>


namespace skk_protocol
{
// #define MYSELF 1

#define SPACE " "
#define SPACE_LEN strlen(SPACE)

#define SEP "\r\n"
#define SEP_LEN strlen(SEP)   // 不能是sizeof！(不包含\0) 

    // 请求  client -> server
    class Request
    {
    public:
        // 1.自主实现   "length\r\n_x _op _y\r\n"
        // 2.使用现成的方案  json

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
            Json::Value root;   // 万能对象
            root["x"] = _x;
            root["y"] = _y;
            root["op"] = _op;
            Json::FastWriter writer;

            return writer.write(root);   // 返回序列化后的string

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
            Json::Value root;
            Json::Reader reader;
            reader.parse(str, root);   // 反序列化写到root中
            _x = root["x"].asInt();   // 以int类型，把x的数据写到_x
            _y = root["y"].asInt();
            _op = root["op"].asInt();   // 以 ASCII码传入的，所以转int就行
            
            return true; 
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
        // 约定
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
#ifdef MYSELF
            std::string str;
            str = std::to_string(_code);
            str += SPACE;
            str += std::to_string(_result);
            return str;
#else
            Json::Value root;
            root["code"] = _code;
            root["result"] = _result;
            root["xx"] = _x;
            root["yy"] = _y;
            root["zz"] = _op;

            Json::FastWriter writer;
            
            return writer.write(root);  // 返回序列化后的string

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
            Json::Value root;
            Json::Reader reader;
            reader.parse(str, root);     // 反序列化写到root中
            _code = root["code"].asInt();   // 以int类型，写入数据
            _result = root["result"].asInt();
            _x = root["xx"].asInt();
            _y = root["yy"].asInt();
            _op = root["zz"].asInt();
   
            return true; 
#endif                          
        }

    public: 
        Response()
        {}

        Response(int result, int code, int x, int y, char op)
            :_result(result)
            ,_code(code)
            ,_x(x)
            ,_y(y)
            ,_op(op)
        {}

        ~Response(){}
    public:
        int _result;  // 计算结果  
        int _code;    // 计算结果的状态码  0正确

        int _x;
        int _y;
        int _op;
    };

    // 读请求  (临时方案)
    // 调整方案2：必须返回一个完整的报文
    bool Recv(int sock, std::string* out)
    {
        // UDP 面向数据报 -- 发一个收一个
        // TCP 面向字节流 -- 发多次，收的时候一次性收完
        // recv: 怎么保证读到的inbuffer是一个完整完善的请求呢？
        // 发"_x _op _y" --> 可能收 "_x _o"  "p _y"
        char buffer[1024];
        ssize_t s = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (s > 0)
        {
            buffer[s] = 0;
            *out += buffer;
        }
        else if(s == 0)
        {
            // 客户端被关闭了
            // std::cout << "client quit" << std::endl;
            return false;
        }
        else
        {
            // 读取错误
            // std::cout << "recv error" << std::endl;
            return false;
        }
        
        return true;
        
    }

    // 发送数据
    void Send(int sock, const std::string str)
    {
        //std::cout << "send in" << std::endl;
        int n = send(sock, str.c_str(), str.size(), 0);
        if (n < 0)
        {
            std::cout << "send error" << std::endl;
        }
    }

    // 协议解析，保证得到一个完整的报文
    // "length\r\n_x _op _y\r\n"
    std::string Decode(std::string& buffer)
    {
        std::size_t pos = buffer.find(SEP);
        if (pos == std::string::npos)
        {
            return "";  // 返回空串
        }
        int size = atoi(buffer.substr(0, pos).c_str());   // length数据，字符串转int

        int surplus_size = buffer.size() - pos - (2 * SEP_LEN);  // 剩余内容的size
        if (surplus_size >= size)
        {
            // 至少具有一个合法完整的报文，可以提取了
            buffer.erase(0, pos + SEP_LEN);          // 删除开头的 "length\r\n"
            std::string s = buffer.substr(0, size);   // 截取"_x _op _y"
            buffer.erase(0, size + SEP_LEN);        // 删除 "_x _op _y\r\n"
            return s;
        }
        else
        {
            return "";  // 返回空串
        }
    }

    // 添加协议结构
    // "XXXXX"
    // "lenth\r\nXXXXX\r\n"
    std::string Encode(std::string& s)
    {
        std::string new_package = std::to_string(s.size());
        new_package += SEP;
        new_package += s;
        new_package += SEP;
        return new_package;
    }

}



