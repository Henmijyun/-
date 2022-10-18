#pragma once 

#include <iostream>
#include <vector>

using namespace std;

namespace skk
{
    // 默认 大堆
    template<class T, class Container = vector<T>>
    class priority_queue
    {
    public:

        // O(logN)
        void adjust_up(size_t child) // 向上调整堆
        {
            size_t parent = (child - 1) / 2; // 父节点
            while (child > 0)
            {
                if (_con[child] > _con[parent]) // 孩子>父亲
                {
                    std::swap(_con[child], _con[parent]);
                    child = parent;
                    parent = (child - 1) / 2;  // 交换后，更新父子节点
                }
                else 
                {
                    break;
                }
            }
        }

        void push(const T& x)
        {
            // 把数据插入堆尾中，再向上调整
            _con.push_back(x);
            adjust_up(_con.size() - 1); // 堆的向上调整  size-1是新插入数据的下标
        }

        void adjust_down(size_t parent) // 向下调整堆
        {

        }

        void pop()
        {
            // 先把根节点和堆尾数据交换，再删除，最后向下调整
            std::swap(_con[0], _con[_con.size() - 1]);
            _con.pop_back();

            adjust_down(0); // 从根节点，向下调整堆
        }
    private:
        Container _con;
    };

    void test_priority_queue1()
    {

    }
}









