#pragma once 

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <assert.h>
#include "reverseIterator.h"

using namespace std;

namespace skk
{
    template<class T>
    struct list_node  // 单个节点
    {
        T _data;
        list_node<T>* _next;
        list_node<T>* _prev;

        list_node(const T& x = T())
            : _data(x)
            , _next(nullptr)
            , _prev(nullptr)
        {}
    };
   
    // typedef __list_iterator<T, T&, T*>             iterator;
    // typedef __list_iterator<T, const T&, const T*> const_iterator;
    
    // 像指针一样的对象
    template<class T, class Ref, class Ptr>  // 为了控制const T& 和const T*
    struct __list_iterator  // 迭代器(公有)
    {
        
        typedef list_node<T> Node; 
        typedef __list_iterator<T, Ref, Ptr> iterator; 


        //////////////////////////////
        //这里是为了使用std::find()而增加的源码typedef,与list无关，不用管
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef ptrdiff_t difference_type;
        ////////////////////////////// 


        Node* _node;

        __list_iterator(Node* node)
            :_node(node)
        {}
        
        bool operator!=(const iterator& it) const // 重载!=
        {
            return _node != it._node;
        }

        // *it  it.operator*() 
        // T& operator*()
        Ref operator*()
        {
            return _node->_data; 
        }

        // T* operator->()
        Ptr operator->()
        {
            return &(operator*()); // 相当于 &(_node->_data)
        }
        
        // ++it 
        iterator& operator++()
        {
            _node = _node->_next;
            return *this;
        }

        // it++
        iterator operator++(int)
        {
            iterator tmp(*this);
            _node = _node->_next;
            return tmp;
        }

        // --it 
        iterator& operator--()
        {
            _node = _node->_prev;
            return *this;
        }

        // it--
        iterator operator--(int)
        {
            iterator tmp(*this);
            _node = _node->_prev;
            return tmp;
        }
    };

    template<class T>
    class list 
    {
        typedef list_node<T> Node; 
    public:
        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, const T&, const T*> const_iterator; 
        // 利用模板实例化去控制const T& 和 const T*
        
        typedef __reverse_iterator<iterator, T&, T*> reverse_iterator;
        typedef __reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;

        iterator begin()
        {
            return iterator(_head->_next);
        }

        iterator end()
        {
            return iterator(_head);
        }

        const_iterator begin() const
        {
            return const_iterator(_head->_next);
        }

        const_iterator end() const
        {
            return const_iterator(_head);
        }
        
        reverse_iterator rbegin()
        {
            return reverse_iterator(end()); // 用end返回的迭代器构造一个反向迭代器
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin()); // 同理
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }
        
        
        void empty_init() // 创建 并 初始化 哨兵位的头
        {
            _head = new Node;
            _head->_next = _head;
            _head->_prev = _head;
        }

        template <class InputIterator>
        list(InputIterator first, InputIterator last) // 利用 迭代器区间 进行构造
        {
            empty_init(); // 哨兵位的头

            while (first != last)
            {
                push_back(*first);
                ++first;
            }
        }

        list()
        {
            empty_init(); // 哨兵位的头
        }

        void swap(list<T>& x) // 交换
        {
            std::swap(_head, x._head);       
        }
        
        // lt2(lt1) 现代写法
        list(const list<T>& lt) // 拷贝构造
        {
            empty_init();
            list<T> tmp(lt.begin(), lt.end()); // 出了作用域，自动析构
            swap(tmp);

        }
        
        // lt1 = lt3 
        list<T>& operator=(list<T> lt) 
        {
            swap(lt);  // lt相当于lt3的深拷贝，对传值传参会拷贝构造，再进行交换
            return *this;  // 交换后函数结束，lt自动析构 （两全其美）
        }

        ~list()
        {
            clear();
            delete _head;
            _head = nullptr;
        }

        void clear() // 清空链表，但不销毁
        {
            iterator it = begin();
            while (it != end())
            {
                it = erase(it);
            }
        }
        
        void push_back(const T& x) // 尾插
        {
          //  Node* tail = _head->_prev;
          //  Node* newnode = new Node(x);

          //  // _head       tail  newnode
          //  tail->_next = newnode;
          //  newnode->_prev = tail;
          //  newnode->_next = _head;
          //  _head->_prev = newnode;
          
            insert(end(), x);
         }

        void push_front(const T& x) // 头插
        {
            insert(begin(), x);
        }

        iterator insert(iterator pos, const T& x) // 指定位置插入
        {
            Node* cur = pos._node;    // 取迭代器pos里面的节点位置
            Node* prev = cur->_prev; 

            Node* newnode = new Node(x);
            // prev  newnode  cur 
            prev->_next = newnode;
            newnode->_prev = prev;
            newnode->_next = cur;
            cur->_prev = newnode;

            return iterator(newnode); // 返回新插入数据位置的迭代器 
            // 利用iterator类，去构造一个迭代器，再返回

        }

        void pop_back() // 尾删
        {
            erase(--end()); // 头节点的prev
        }

        void pop_front() // 头删
        {
            erase(begin());
        }

        iterator erase(iterator pos) // 指定位置删除
        {
            assert(pos != end());

            Node* cur = pos._node;
            Node* prev = cur->_prev;
            Node* next = cur->_next;

            prev->_next = next;
            next->_prev = prev;
            delete cur;

            return iterator(next); // 返回删除的pos后面的位置next的迭代器
        }



    private:
        Node* _head;  // 哨兵位的头
    };
    
    void test_list1()
    {
        list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);
        lt.push_back(4);
        lt.push_back(5);

        list<int>::iterator it = lt.begin(); // 迭代器
        while (it != lt.end())
        {
            cout << *it << " ";
            ++it;
        }
        cout << endl;
        
    }

    void Func(const list<int>& l)
    {
        list<int>::const_iterator it = l.begin();
        while (it != l.end())
        {
            cout << *it << " ";
            ++it;
        }
        cout << endl;
    }

    void test_list2()
    {
        list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);
        lt.push_back(4);
        lt.push_back(5);
        
        Func(lt);
        
        for (auto& e : lt)
        { 
            e *= 2;
            cout << e << " ";
        }
        cout << endl;

        lt.pop_back();
        lt.pop_front();

        for (auto& e : lt)
        { 
            cout << e << " ";
        }
        cout << endl;
       
        list<int>::iterator pos = find(lt.begin(), lt.end(), 6);
        if (pos != lt.end()) 
        {
            pos = lt.erase(pos); //指定位置删除
            pos = lt.insert(pos, 50); //指定插入
            *pos *= 100;
        }
        for (auto& e : lt)
        { 
            cout << e << " ";
        }
        cout << endl;
    }

    void test_list3()
    {
        list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);
        lt.push_back(4);
        lt.push_back(5);
        
        list<int> copy = lt;
        for (auto& e : copy)
        { 
            e *= 2;
            cout << e << " ";
        }
        cout << endl;
        
        lt = copy;
        for (auto& e : lt)
        { 
            cout << e << " ";
        }
        cout << endl;


        // 反向迭代器
        list<int>::reverse_iterator rit = lt.rbegin();
        while (rit != lt.rend())
        {
            cout << *rit << " "; 
            ++rit;
        }
        cout << endl;
    }

}













