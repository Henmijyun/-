#pragma once 

#include <iostream>
#include <string>
#include <list>
#include <assert.h>
#include <string.h>
using namespace std;

namespace bit
{
    class string
    {
    public:
        typedef char* iterator;
        typedef const char* const_iterator;

        iterator bedin()
        {
            return _str;
        }
        const_iterator bedin() const // 为区分传参对象是不是const
        {
            return _str;
        }

        iterator end()
        {
            return _str + _size; // 最后一个下标+1的位置
        }
        const_iterator end() const
        {
            return _str + _size;
        }


        /*err  不能把字符串构造置为空，应在里面保'\0'
        string(const char* str)
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
            _str = new char[_capacity + 1]; // _capacity里面没有算\0 ，所以+1

            strcpy(_str, str);
        }

        // 传统写法 
        // 深拷贝： 先开一个空间，再把数据拷贝进这个空间里面
        // s2(s1)  拷贝构造
        //string(const string& s)
        //	:_str(new char[s._capacity+1])  // 创建一样大的空间
        //	, _size(s._size)
        //	, _capacity(s._capacity)
        //{
        //	strcpy(_str, s._str);  // 拷贝过去 （这里是深拷贝）
        //}
        
        //// s1 = s3
        //// s1 = s1
        //string& operator=(const string& s)
        //{
        //	if (this != &s)
        //	{
        //		char* tmp = new char[s._capacity + 1];
        //		strcpy(tmp, s._str); // 先拷贝

        //		delete[] _str;  // 再释放

        //		_str = tmp;
        //		_size = s._size;
        //		_capacity = s._capacity;
        //	}

        // 现代写法 - 资本主义
        // s2(s1)
        void swap(string& tmp)
        {
            ::swap(_str, tmp._str);
            ::swap(_size, tmp._size);
            ::swap(_capacity, tmp._capacity);
        }

        // s2(s1)
        string(const string& s)
            :_str(nullptr)  // 初始化列表把s2初始化了，
            ,_size(0)       // s2和tmp交换后，tmp的释放s2的nullptr
            ,_capacity(0)
        {
            string tmp(s._str);  // 用构造函数，相当于string tmp("hello")
            swap(tmp); // 交换   this->swap(tmp);
        }

        // s1 = s3  传统写法
        //string& operator=(const string& s)
        //{
        //    if (this != &s) // 两个地址对比
        //    {
        //        //string tmp(s._str); // 复用 构造
        //        string tmp(s); // 复用 拷贝构造
        //        swap(tmp);  // this->swap(tmp);
        //    }
        //    return *this;
        //}

        // s1 = s3  现代写法
        // s顶替tmp做打工人
        string& operator=(string s) // 直接用传值传参的拷贝，来做打工人
        {
            swap(s);  
            // 必须用自己实现的swap，因为全局的swap底层也是复用运算符重载的=
            // 会导致死循环，无法执行
            return *this;
        }

        ~string()
        {
            delete[] _str;
            _str = nullptr;
            _size = _capacity = 0;
        }

        const char* c_str() const
        {
            return _str; // 返回值自动强转为const char*
        }
        
        void clear()
        {
            _str[0] = '\0';
            _size = 0;
        }

        size_t size() const
        {
            return _size;
        }

        size_t capacity() const
        {
            return _capacity;
        }

        const char& operator[](size_t pos) const
        {
            assert(pos < _size);

            return  _str[pos];
        }

        char& operator[](size_t pos)
        {
            assert(pos < _size);

            return _str[pos];
        }

        void reserve(size_t n) // 扩容
        {
            if (n > _capacity) // 判断是否扩容
            {
                char* tmp = new char[n + 1];
                strcpy(tmp, _str);
                delete[] _str;

                _str = tmp;
                _capacity = n;
            }
        }

        void push_back(char ch) // 尾插字符
        {
            // 满了就扩容
            if (_size == _capacity)
            {
                reserve(_capacity == 0 ? 4 : _capacity * 2);
            }

            _str[_size] = ch;
            ++_size;
            _str[_size] = '\0';  // 记得结尾补\0
        }

        void append(const char* str) //尾插字符串
        {
            size_t len = strlen(str);
            
            // 尾插之后装不下的话，先扩容
            if (_size + len > _capacity)
            {
                reserve(_size + len);
            }

            strcpy(_str + _size, str);
            //strcat(_str, str); 需要遍历找\0，效率低
            _size += len;

        }

        /* // 重载只需在原有函数的基础上，复用则可实现
        void append(const string& s)
        {
            append(s._str);
        }

        void append(size_t n, char ch)
        {
            reserve(_size + n);
            for (size_t i = 0; i < n; ++i)
            {
                push_back(ch); // 尾插n个ch
            }
        }
        */
        
        string& operator+=(char ch)
        {
            push_back(ch);
            return *this;
        }

        string& operator+=(const char* str)
        {
            append(str);
            return *this;
        }

        string& insert(size_t pos, char ch)
        {
            assert(pos <= _size);
            
            if (_size == _capacity) // 满了就扩容
            {
                reserve(_capacity == 0 ? 4 : _capacity * 2);
            }

            //// 挪动数据
            //int end = _size;
            //while (end >= (int)pos) // 需要强转来实现
            //{
            //	_str[end + 1] = _str[end];
            //	--end;
            //}

            size_t end = _size + 1;
            while (end > pos)  // 从末尾逐个数据向pos下标挪动
            {
                _str[end] = _str[end - 1];
                --end;
            }

            _str[pos] = ch;
            ++_size;

            return *this;
        }

        string& insert(size_t pos, const char* str)
        {
            assert(pos <= _size);
            size_t len = strlen(str);
            if (_size == _capacity)
            {
                reserve(_capacity == 0 ? 4 : _capacity * 2);
            }

            // 挪动数据
            size_t end = _size + len; // 插入后的末尾下标
            while (end > pos)
            {
                // 把原字符串 的中间位置 空出 要插入的字符串空间
                _str[end] = _str[end - len]; 
                --end;
            }

            strncpy(_str + pos, str, len); // 拷贝数据
            _size += len;

            return *this;
        }

        void erase(size_t pos, size_t len = npos) // 删除
        {
            assert(pos < _size);

            if (len == npos || (pos + len) >= _size)
            {
                _str[pos] = '\0';
                _size = pos;
            }
            else
            {
                strcpy(_str + pos, _str + pos + len); // 把后面剩下的字符往前面覆盖
                _size -= len;
            }

        }
        

        size_t find(char ch, size_t pos = 0) const // 查找字符，返回下标
        {
            assert(pos < _size);

            for (size_t i = pos; i < _size; ++i)
            {
                if (ch == _str[i])
                {
                    return i;
                }
            }

            return npos;
        }

        size_t find(const char* sub, size_t pos = 0) const // 查找字符串
        {
            assert(pos < _size);
            assert(sub);
            
            // kmp / bm 算法都可以实现，bm更优
            const char* ptr = strstr(_str + pos, sub); // strstr函数暴力查找
            if (ptr == nullptr)
            {
                return npos; // 找不到
            }
            else
            {
                return ptr - _str; // 指针减指针 -> 下标
            }
        }

        // 从字符串中 截取 部分字符串
        string substr(size_t pos, size_t len = npos) const 
        {
            assert(pos < _size);

            size_t realLen = len;
            if (len == npos || (len + pos) > _size)
            {
                realLen = _size - pos; // 需要截取的字符个数
            }

            string sub;
            for (size_t i = 0; i < realLen; ++i)
            {
                sub += _str[pos + i]; // 拷贝
            }

            return sub;
        }

        bool operator>(const string& s) const 
        {
            return strcmp(_str, s._str) > 0;
        }

        bool operator==(const string& s) const 
        {
            return strcmp(_str, s._str) == 0;
        }

        bool operator>=(const string& s) const 
        {
            return *this > s || *this == s;
        }

        bool operator<=(const string& s) const 
        {
            return !(*this > s);
        }

        bool operator<(const string& s) const 
        {
            return !(*this >= s);
        }

        bool operator!=(const string& s) const 
        {
            return !(*this == s);
        }

    private:
        char* _str;
        size_t _size;
        size_t _capacity;

        // const static 语法特殊处理
        // 直接可以当成定义初始化
        const static size_t npos = -1;
    };
    //size_t string::npos = -1; // 本来是要在类外面定义的，但是如果加了const，就不用了

    ostream& operator<<(ostream& out, const string& s)
    {
        for (size_t i = 0; i < s.size(); ++i)
        {
            out << s[i];
        }
        return out;
    }

    /* // 这个方法效率不够高
    istream& operator>>(istream& in, string& s)
    {
        //  如果输入字符串很长，不断+=，频繁扩容，效率很低，需要优化一下 
        char ch;
        //in >> ch; // 无法识别空格和回车
        ch = in.get();
        s.reserve(128); // 可以预先把空间开好，但是如果用不上就浪费了
        while (ch != ' ' && ch != '\n')
        {
            s += ch;
            ch = in.get();
        }

        return in;
    }
    */
    
    // 优化版
    istream& operator>>(istream& in, string& s)
    {
        s.clear();

        char ch;
        ch = in.get(); // 能识别空格

        const size_t N = 32;
        char buff[N]; 
        size_t i = 0;

        while (ch != ' ' && ch != '\n')
        {
            buff[i++] = ch; // 把输入的字符先保存到数组
            if (i == N-1)
            {
                buff[i] = '\0';
                s += buff;  // 数组满了才把数据拷贝到对象里（减少拷贝的次数）
                i = 0;
            }

            ch = in.get();
        }
        
        if (i != 0) // 如果数组里还有数据，就拷贝到对象
        {
            buff[i] = '\0'; 
            s += buff; 
        }
        
        return in;
    }


    void TestString1()
    {
        string s1("hello");
        string s2(s1);
        string s3 = s1; // 坚持 = 和 拷贝构造

        cout << s2.c_str() << " " << s3.c_str() << endl << endl;

        s2 = "xxxxxxx";
        s1.swap(s2);  // 交换
        cout << s1.c_str() << " " << s2.c_str() << endl;
        swap(s1, s2);
        cout << s1.c_str() << " " << s2.c_str() << endl << endl;

        s1.push_back('Q'); // 插入
        s2.append("QQQ");
        cout << s1.c_str() << " " << s2.c_str() << endl << endl;

        s1.insert(2, "XXX"); // 指定插入
        s2.insert(2, 'X');
        cout << s1.c_str() << " " << s2.c_str() << endl << endl;

        s1.erase(5, 2);  // 删除
        s2.erase(5);
        cout << s1.c_str() << " " << s2.c_str() << endl << endl;

    }

    void TestString2()
    {
        /*	string s1;
            cin >> s1;
            cout << s1 << endl;*/

        string s1("hello");
        cout << s1 << endl;
        cout << s1.c_str() << endl;
        s1 += '\0';
        s1 += "world";
        cout << s1 << endl;
        cout << s1.c_str() << endl;

        string s3, s4;
        cin >> s3 >> s4;
        cout << s3 << " -- " << s4 << endl;
    }

}
