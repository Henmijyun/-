#pragma once 

namespace skk
{
    // 反向迭代器
    template<class Iterator, class Ref, class Ptr>
    struct __reverse_iterator
    {
        Iterator _cur;

        typedef __reverse_iterator<Iterator, Ref, Ptr> RIterator;

        __reverse_iterator(Iterator it) // 用传过来的迭代器进行构造
            :_cur(it)
        {}

        RIterator operator++()
        {
            --_cur;
            return *this;
        }

        RIterator operator--()
        {
            ++_cur;
            return *this;
        }

        Ref operator*() // 因为rbegin()的构成是用end();
        {
            auto tmp = _cur;
            --tmp;
            return *tmp;
        }

        Ptr operator->()
        {
            return &(operator*()); 
        }

        bool operator!=(const RIterator& it)
        {
            return _cur != it._cur; 
        }
    };
}



