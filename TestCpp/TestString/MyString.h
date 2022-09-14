#pragma once 

namespace bit
{
#include <string.h>
    class string
    {
    public:
        typedef char* iterator;
        typedef const char* const_iterator;

        iterator bedin()
        {
            return _str;
        }
        const_iterator bedin()const // 为区分传参对象是不是const
        {
            return _str;
        }

        iterator end()
        {
            return _str + _size; // 最后一个下标+1的位置
        }
        const_iterator end()const 
        {
            return _str + _size;
        }


        /*err  不能把字符串构造置为空，应在里面保'\0'
         * string(const char* str)
            :_str(nullptr), 
            _size(0),
            _capacity(0);
        {}  // 不使用或半使用初始化列表，有助于工程后期修改
            // 在函数内定义，不用在意初始化顺序
        */ 
        
        string(const char* str = "") // 空字符串里面自带\0
        {
            _size = strlen(str);
            _capacity = _size;
            _str = new char[_capacity + 1]; // _capacity里面没有算\0
        
            strcpy(_str, str);     
        }


    private:
        char* _str;
        size_t _size;
        size_t _capacity;
    };
}

















