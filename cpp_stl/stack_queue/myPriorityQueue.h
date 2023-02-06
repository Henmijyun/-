#pragma once 

#include <iostream>
#include <vector>
#include <functional> // less 大堆 / greater 小堆

using namespace std;

namespace skk
{
    // 默认 大堆
    // Compare是进行比较的仿函数： less->大堆 / greater->小堆   
    template<class T, class Container = vector<T>, class Compare = std::less<T>>
    class priority_queue
    {
    public:
        priority_queue()
        {} // 初始化阶段，自动调用Container的缺省或传参，进行构造

        template<class InputIterator>
        priority_queue(InputIterator first, InputIterator last) // 用迭代器期间来构造
        {
            while (first != last) // 先把数据放进去
            {
                _con.push_back(*first);
                ++first;
            }

            // 再建堆
            for (int i = (_con.size()-1-1) / 2; i >= 0; --i)
            {
                adjust_down(i); // 向下调整建堆 （效率更高）
            }
        }

        // O(logN)
        void adjust_up(size_t child) // 向上调整堆
        {
            Compare com; // 创建一个仿函数对象
            size_t parent = (child - 1) / 2; // 父节点
            while (child > 0) // 孩子下标 > 根节点
            {
                // if (_con[child] > _con[parent]) // 孩子>父亲
                // if (_con[parent] < _con[child]) // 孩子>父亲
                if (com(_con[parent],  _con[child])) // 孩子>父亲 仿函数
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

        // O(logN)
        void adjust_down(size_t parent) // 向下调整堆
        {
            Compare com; // 创建一个仿函数对象
            size_t child = parent * 2 + 1;  // 孩子节点
            while (child < _con.size()) // 孩子下标 < 数组最后一个下标 
            {
                // 选出左右孩子中，大的那一个
                // 右孩子存在，并大于左孩子
               // if (child + 1 < _con.size() && _con[child + 1] > _con[child])
               // if (child + 1 < _con.size() && _con[child] < _con[child + 1])
                if (child + 1 < _con.size() && com(_con[child], _con[child + 1])) // 仿函数
                {
                    ++child; // 下标指向右孩子
                }

                // if (_con[child] > _con[parent]) // 孩子 > 父节点
                // if (_con[parent] < _con[child]) // 孩子 > 父节点
                if (com(_con[parent], _con[child])) // 仿函数
                {
                    std::swap(_con[child], _con[parent]);
                    parent = child;
                    child = parent * 2 + 1;
                }
                else 
                {
                    break;
                }
            }
        }

        void pop()
        {
            // 先把根节点和堆尾数据交换，再删除，最后向下调整
            std::swap(_con[0], _con[_con.size() - 1]);
            _con.pop_back();

            adjust_down(0); // 从根节点，向下调整堆
        }

        const T& top()
        {
            return _con[0];
        }

        bool empty() const 
        {
            return _con.empty();
        }

        size_t size() const 
        {
            return _con.size();
        }


    private:
        Container _con;
    };

    void test_priority_queue1()
    {
        priority_queue<int> pq; // Container的缺省值为 vector<int>

        pq.push(3);
        pq.push(5);
        pq.push(2);
        pq.push(4);
        pq.push(1);

        while (!pq.empty())
        {
            cout << pq.top() << " ";
            pq.pop();
        }
        cout << endl;
    
        int a[] = { 3, 2, 7, 6, 0, 4, 1, 9, 8, 5  };
        //prociority_queue<int> heap(a, a + sizeof(a) / sizeof(int)); // 用迭代器区间构造
        priority_queue<int, vector<int>, greater<int>> heap(a, a + sizeof(a) / sizeof(int));
        while (!heap.empty())
        {
            cout << heap.top() << " ";
            heap.pop();
        }
        cout << endl;
    }
}









