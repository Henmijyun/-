#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <string.h>
#include "reverseIterator.h"

using namespace std;


namespace skk
{
	template <class T>  // 模板
	class vector
	{
		
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

        typedef __reverse_iterator<iterator, T&, T*> reverse_iterator;
        typedef __reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;

		iterator begin()
		{
			return _start;
		}

		const_iterator begin() const
		{
			return _start;
		}
		
		iterator end()
		{
			return _finish; // 最后一个数据+1的位置
		}

		const_iterator end() const
		{
			return _finish; 
		}

		reverse_iterator rbegin() // 反向迭代器
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}
		
		reverse_iterator rend()
		{
			return reverse_iterator(begin()); 
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin()); 
		}



		vector()
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{}

        vector(size_t n, const T& val = T()) // 用n个T类型的数据，进行构造，缺省值为T()
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
        {
            reserve(n); // 扩容
            for (size_t i = 0; i < n; ++i)
            {
                push_back(val); // 插入n个T类型的数据
            }
        }

        
        vector(int n, const T& val = T()) // 用n个T类型的数据，进行构造，缺省值为T()
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
        {
            reserve(n); // 扩容
            for (int i = 0; i < n; ++i)
            {
                push_back(val); // 插入n个T类型的数据
            }
        }

		// v2(v1)
		/*vector(const vector<T>& v) // 拷贝构造（传统深拷贝）
		{
			_start = new T[v.size()]; // v.capacity()也可以
			// memcpy(_start, v._start, sizeof(T) * v.size()); // 拷贝数据
			// memcpy()当遇到vector<vector<int>> val; 这种情况，只会深拷贝上面一层，
            // 下面一层是浅拷贝，所以析构的时候会崩溃
            //
            for (size_t i = 0; i < v.size(); ++i)
            {
                _start[i] = v._start[i];
            }
            _finish = _start + v.size();
			_end_of_storage = _start + v.size();
		}*/ 

		/*vector(const vector<T>& v)
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			reserve(v.size());
			for (const auto& e : v)
			{
				push_back(e);
			}
		}*/


		template <class InputIterator>   // 模板可以嵌套        
		vector(InputIterator first, InputIterator last)  // 使得可以用其他类来实现 
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}

		void swap(vector<T>& v) 
		{
			// 直接拷贝是深拷贝，这里分开一个个的拷贝，相当于内置类型拷贝
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storage, v._end_of_storage);
		}

		// v2(v1)
		vector(const vector<T>& v) // 传参必须引用& 否则是无限递归
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			vector<T> tmp(v.begin(), v.end());
			swap(tmp);
		}
        
        // v2 = v1 
        vector<T>& operator=(vector<T> v) // 传参时会深拷贝，直接对这个深拷贝进行交换则可
        {
            swap(v);
            return *this;
        }

		~vector()
		{
			delete[] _start;
			_start = _finish = _end_of_storage = nullptr;
		}

		size_t capacity() const  // 空间
 		{
			return _end_of_storage - _start;
		}

		size_t size() const
		{
			return _finish - _start;
		}

		T& operator[](size_t pos) // 运算符重载
		{
			assert(pos < size());
			return _start[pos];
		}

		const T& operator[](size_t pos) const 
		{
			assert(pos < size());
			return _start[pos];
		}

		void reserve(size_t n)  // 扩容
		{
			if (n > capacity())
			{
				size_t sz = size();
				T* tmp = new T[n];
				if (_start) // 不为空就拷贝
				{
					//memcpy(tmp, _start, sizeof(T) * sz);
					// 在vector<vector<int>> val;的情况下，上层的vector是深拷贝，
                    // 而下层的vector是浅拷贝，所以会err

                    for (size_t i = 0; i <  sz; ++i)
                    {
                        tmp[i] = _start[i];
                    }
                    delete[] _start;
				}

				_start = tmp;
				_finish = _start + sz;
				_end_of_storage = _start + n;
			}
		}

		void resize(size_t n, const T& val = T()) // 改变当前vector的大小为n
        {
            if (n > capacity())
            {

                reserve(n);
            }

            if (n > size())
            {  
                // 初始化填值
                while (_finish < _start + n)
                {
                    *_finish = val;
                    ++_finish;
                }
            }
            else
            {
                //  删除数据
                _finish = _start + n;
            }
        }

		void push_back(const T& x)
		{
			/*
			if (_finish == _end_of_storage)
			{
				reserve(capacity() == 0 ? 4 : capacity() * 2);
			}
			*_finish = x;
			++_finish;
			*/

			insert(end(), x); // 复用
		}

		void pop_back()
		{
			assert(_finish > _start);
			--_finish;
		}

		iterator insert(iterator pos, const T& x) // 指定位置插入
		{
			assert(pos >= _start && pos <= _finish);
			
			if (_finish == _end_of_storage)
			{
				size_t len = pos - _start; // 防止迭代器失效
				reserve(capacity() == 0 ? 4 : capacity() * 2);
				pos = _start + len;
			}
			iterator end = _finish - 1; // 取最后一个数据的位置
			while (end >= pos)
			{
				*(end + 1) = *end;  //挪动数据
				--end;
			}

			*pos = x;
			++_finish;

			return pos;
		}

		// STL规定erase返回删除位置下一个位置的迭代器
		iterator erase(iterator pos) // 指定位置删除
		{
			assert(pos >= _start && pos < _finish);

			iterator begin = pos + 1;
			while (begin != _finish)
			{
				*(begin - 1) = *begin;
				++begin;
			}

			--_finish;

			// 可能会有些版本有缩容的情况--以时间换空间
			// 缩容也会使当前迭代器失效
			return pos;
		}
        
        T& front() // 返回头位置的数据
        {
            assert(size() > 0);

            return *_start;
        }

        T& back()  // 返回尾位置的数据
        {
            assert(size() > 0);

            return *(_finish - 1);
        }


	private:
		iterator _start;
		iterator _finish;
		iterator _end_of_storage;
	};


	void Func(const vector<int>& v)  // 只能读，不能改
	{
		vector<int>::const_iterator it = v.begin();
		while (it != v.end()) 
		{
			cout << *it << " ";  
			++it;
		}
		cout << endl;
	}

	void TestVector1()
	{
		vector<int> n;
		n.push_back(1);
		n.push_back(2);
		n.push_back(3);
		n.push_back(4);
		n.push_back(5);

		vector<int>::iterator it = n.begin();  // 迭代器
		while (it != n.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;

		n.pop_back();
		Func(n);  // const只读版

		auto pos = find(n.begin(), n.end(), 3); // 查找对应数据，获取位置
		if (pos != n.end())  // 查找失败的话返回end位置
		{
			// 在pos位置插入数据以后，不要访问pos，因为pos可能失效了。
			pos = n.insert(pos, 7); // 指定位置插入
			++pos;
			++pos;
		}
		else
		{
			++pos;
		}

		for (auto e : n) // 范围for
		{
			cout << e << " ";
		}
		cout << endl;


	}

	void TestVector2()
	{
		vector<int> n;
		n.push_back(1);
		n.push_back(2);
		n.push_back(3);
		n.push_back(4);
		n.push_back(5);

		auto pos = find(n.begin(), n.end(), 1); 
		if (pos != n.end()) 
		{
			pos = n.erase(pos); // 指定位置删除
		}
		else
		{
			++pos;
		}

		for (auto e : n) 
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void TestVector3()
	{
		vector<int> n;
		n.push_back(1);
		n.push_back(2);
		n.push_back(3);
		n.push_back(4);
		n.push_back(5);
		for (auto e : n)
		{
			cout << e << " ";
		}
		cout << endl;

		vector<int> v(n);
		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;
	}

	void TestVector4()
	{
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);
		for (auto e : v)
		{
			cout << e << " ";
		}
		cout << endl;

        string s("hello word");
        vector<int> vs(s.begin(), s.end()); // 把s的内存区间进行构造

		for (auto e : vs)
		{
			cout << e << " ";
		}
		cout << endl;

        //vs = v; // 已经存在的两个对象，赋值拷贝
        vector<int> copy = vs; // 等于copy(v); 拷贝构造 （copy本来不存在）
        
		for (auto e : copy)
		{
			cout << e << " ";
		}
		cout << endl;


        vector<int>::reverse_iterator rit = v.rbegin(); // 反向迭代器
        while (rit != v.rend())
        {
            cout << *rit << " ";
            ++rit;
        }
        cout << endl;
    }








}













