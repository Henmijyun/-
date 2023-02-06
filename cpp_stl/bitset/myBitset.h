#pragma once 

#include <iostream>
#include <vector>

using namespace std;

namespace skk
{
    template<size_t N> // 非类型模板参数
    class bitset
    {
    public:
        bitset()
        {
            _bits.resize(N/8+1, 0); // 开空间
        }

        void set(size_t x)
        {
            size_t i = x / 8;
            size_t j = x % 8;

            _bits[i] |= (1 << j);
        }

        void reset(size_t x)
        {
            size_t i = x / 8;
            size_t j = x % 8;

            _bits[i] &= ~(1 << j);
        }

        bool test(size_t x)
        {
            size_t i = x / 8;
            size_t j = x % 8;

            return _bits[i] & (1 << j);  // 1 1得真 
        }
    private:
        vector<char> _bits;
    };

    void test_bitset1()
    {
        bitset<100> bs1;
        bs1.set(2);
        bs1.set(9);
        bs1.set(14);

        cout << bs1.test(2) << endl;
        cout << bs1.test(9) << endl;
        cout << bs1.test(14) << endl;

        bs1.reset(2);
        bs1.reset(9);
        bs1.reset(14);

        cout << bs1.test(2) << endl;
        cout << bs1.test(9) << endl;
        cout << bs1.test(14) << endl;
    }

    void test_bitset2()
    {
        // 开int整型大小的空间
        // bitset<-1> bs1; // 推荐 VS可以，但g++会报错
        bitset<0xffffffff> bs2; // 也行
    }


    template<size_t N>
    class twobitset
    {
    public:
        void set(size_t x)
        {
            bool inset1 = _bs1.test(x);
            bool inset2 = _bs2.test(x);

            // 00
            if (inset1 == false && inset2 == false)
            {
                // -> 01
                _bs2.set(x);
            }
            else if (inset1 == false && inset2 == true)
            {
                // -> 10
                _bs1.set(x);
                _bs2.reset(x);
            }
            else if (inset1 == true && inset2 == false)
            {
                // -> 11
                _bs1.set(x);
                _bs2.set(x);
            }
        }

        void print_once_num()
        {
            for (size_t i = 0; i < N; ++i)
            {
                if (_bs1.test(i) == false && _bs2.test(i) == true)
                {
                    cout << i << endl;
                }
            }
        }
    private:
        bitset<N> _bs1;
        bitset<N> _bs2;
    };

    void test_bitset3()
    {
        int a[] = { 3, 4, 5, 2, 3, 4, 4, 4, 4, 12, 77, 65, 44, 4, 44, 99, 33, 33, 33, 6, 5, 34, 12  };

        twobitset<100> bs;
        for (auto e : a)
        {
            bs.set(e);
        }

        bs.print_once_num();
    }
}




