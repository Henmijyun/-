#include <iostream>
#include <functional>
#include "log.hpp"

typedef std::function<int(int, int)> func_t;

class Task
{
public:
    Task()
    {}
    
    Task(int x, int y, func_t func)
        :_x(x)
        ,_y(y)
        ,_func(func)
    {}

    void operator()(const std::string& name)
    {
        // std::cout << "线程 " << name << "处理完成, 结果是: " << _x << "+" << _y << "=" << _func(_x, _y) << std::endl;
        logMessage(WARNING, "%s %d+%d=%d | %s | %d", 
        name.c_str(), _x, _y, _func(_x, _y), __FILE__, __LINE__);
        // __FILE__, __LINE__  预处理符
    }
    
public:
    int _x;
    int _y;
    // int type;
    func_t _func;
};