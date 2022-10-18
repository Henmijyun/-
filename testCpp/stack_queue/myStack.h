#pragma once 

#include <iostream>
#include <vector>
#include <list>
#include <deque> // 类似vector和list的结合体，但是性能平庸

using namespace std;

namespace skk
{

    template<class T, class Container = deque<T>> // 缺省值给deque
    class stack 
    {
    public:
        void push(const T& x) 
        {
            _con.push_back(x);
        }

        void pop() 
        {
            _con.pop_back();
        }

        T& top()
        {
            return _con.back(); // 取尾部数据
        }

        const T& top() const 
        {
            return _con.back();
        }

        bool empty() const // 判空
        {
            return _con.empty();
        }

        size_t size() const 
        {
            return _con.size();
        }
    private:
       // vector<T> _con; 
         Container _con; 
    };

    void test_stack1()
    {
        //stack<int, vector<int>> st; // 用vector
        //stack<int, list<int>> st;  // 用list
        stack<int> st;  // 用缺省值的deque
        st.push(1);
        st.push(2);
        st.push(3);
        st.push(4);
        st.push(5);

        while (!st.empty())
        {
            cout << st.top() << endl;
            st.pop();
        }
        cout << endl;
    }
}





