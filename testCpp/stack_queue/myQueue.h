#pragma once 

#include <iostream>
#include <vector>
#include <list>
#include <deque>

using namespace std;

namespace skk
{
    template<class T, class Container = deque<T>> 
    class queue
    {
    public:
        void push(const T& x)
        {
            _con.push_back(x);
        }

        void pop()
        {
            _con.pop_front(); // 头删
        }

        T& back() // 取尾
        {
            return _con.back();
        }

        T& front() // 取头
        {
            return _con.front();
        }

        const T& back() const 
        {
            return _con.back();
        }

        const T& front() const 
        {
            return _con.front();
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

    void test_queue1()
    {
        queue<int, list<int>> q;  // 用list , 不能用vector,因为不支持头删
        //queue<int> q;  // 用缺省值的deque
        q.push(1);
        q.push(2);
        q.push(3);
        q.push(4);
        q.push(5);
                    
        while (!q.empty())
        {                
            cout << q.front() << endl; 
            q.pop();
        }
        cout << endl;
    }
}















