#pragma once 

#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;

template<class K>
struct HashFunc   // 仿函数 , 为了可以给不同类型取模 
{
    size_t operator()(const K& key)
    {
        return (size_t)key;                
    }
};

template<>
struct HashFunc<string>   // 仿函数 , 特化
{
    // BKDR哈希             
    size_t operator()(const string& key)
    {
        size_t val = 0;
        for (auto ch : key)
        {
            val *= 131; //abc，acb，相加是相等的，为了区分它们,*= 131
            val += ch;                                                                
        }
        return val;
    }
};



// 开散列 -- 拉链法、哈希桶
namespace HashBucket 
{
    template<class T>    
    struct HashNode 
    {
        T _data;
        HashNode<T>* _next;

        HashNode(const T& data)
            :_data(data)
            ,_next(nullptr)
        {}
    };


    ///////////////////////////////////////////////////////////////// 
    // iterator 
    
    // 前置声明
    template<class K, class T, class Hash, class KeyOfT>
    class HashTable; 

    // 迭代器 (单向)
    template<class K, class T, class Hash, class KeyOfT>
    struct __HashIterator
    {
        typedef HashNode<T> Node;
        typedef HashTable<K, T, Hash, KeyOfT> HT;
        typedef __HashIterator<K, T, Hash, KeyOfT> Self;
    
        Node* _node;
        HT* _pht;

        __HashIterator(Node* node, HT* pht)
            :_node(node)
            ,_pht(pht)
        {}


        T& operator*()
        {
            return _node->_data;
        }

        T* operator->()
        {
            return &_node->_data;
        }

        Self& operator++()
        {
            if (_node->_next)
            {
                // 在当前桶中迭代
                _node = _node->_next;
            }
            else 
            {
                // 找下一个桶
                Hash hash;
                KeyOfT kot;
                size_t i = hash(kot(_node->_data)) % _pht->_tables.size(); // 当前桶下标
                ++i;
                for (; i < _pht->_tables.size(); ++i)
                {
                    if (_pht->_tables[i])
                    {
                        _node = _pht->_tables[i];
                        break;
                    }
                }

                // 说明后面没有有数据的桶
                if (i == _pht->_tables.size())
                {
                    _node = nullptr;
                }
            }

            return *this;
        }
   
        bool operator!=(const Self& s) const 
        {
            return _node != s._node;
        }

        bool operator==(const Self& s) const 
        {
            return _node == s._node;
        }
    };

    /////////////////////////////////////////////////////////// 
    // HashTable 

    template<class K, class T, class Hash, class KeyOfT>
    class HashTable 
    {
        typedef HashNode<T> Node;

        template<class k, class t, class hash, class keyoft> // g++环境下，类模板的名字和友元模板的名字会发生冲突，导致编译器无法辨别。因此这里模板用小写字母
        friend struct __HashIterator;

    public:
        typedef __HashIterator<K, T, Hash, KeyOfT> iterator;
       
        //HashTable()
        iterator begin()
        {
            for (size_t i = 0; i < _tables.size(); ++i)
            {
                if (_tables[i])
                {
                    return iterator(_tables[i], this);
                }
            }

            return end();
        }

        iterator end()
        {
            return iterator(nullptr, this);
        }

        ~HashTable()
        {
            // 释放桶空间
            for (size_t i = 0; i < _tables.size(); ++i)
            {
                Node* cur = _tables[i];
                while (cur)
                {
                    Node* next = cur->_next;
                    delete cur;
                    cur = next;
                }
                _tables[i] = nullptr;
            }

            _size = 0;
        }


        inline size_t __stl_next_prime(size_t n) // 素数可以减少负载因子冲突
        {
            // 素数表
            static const size_t __stl_num_primes = 28;  // static const 只有整型int可以给缺省值
            static const size_t __stl_prime_list[__stl_num_primes] =
            {
                53,         97,         193,       389,       769,
                1543,       3079,       6151,      12289,     24593,
                49157,      98317,      196613,    393241,    786433,
                1572869,    3145739,    6291469,   12582917,  25165843,
                50331653,   100663319,  201326611, 402653189, 805306457,
                1610612741, 3221225473, 4294967291
            };

            //const size_t* first = __stl_prime_list;
            //const size_t* last = __stl_prime_list + __stl_num_primes;
            //const size_t* pos = lower_bound(first, last, n);  // 在区间中，找第一个比n大的值
            //return pos == last ? *(last - 1) : *pos;
            for (size_t i = 0; i < __stl_num_primes; ++i)
            {
                if (__stl_prime_list[i] > n)
                {
                    return __stl_prime_list[i];
                }
            }

            return -1;
        }


        ////////////////////////////////////////////////////////// 
        // modify 
        pair<iterator, bool> Insert(const T& data)
        {
            Hash hash;
            KeyOfT kot;
            
            // 去重
            iterator ret = Find(kot(data));
            if (ret != end())
            {
                // 如果值已经存在，返回这个值的迭代器
                return make_pair(ret, false);
            }

            // 负载因子到1就扩容
            if (_size == _tables.size())
            {
                vector<Node*> newTables;
                newTables.resize(__stl_next_prime(_tables.size()), nullptr);
                // 旧表中节点移动映射新表
                for (size_t i = 0; i < _tables.size(); ++i)
                {
                    Node* cur = _tables[i];
                    while (cur)
                    {
                        Node* next = cur->_next;

                        size_t hashi = hash(kot(cur->_data)) % newTables.size();
                        cur->_next = newTables[hashi];
                        newTables[hashi] = cur;
                            
                        cur = next;
                    }


                    _tables[i] = nullptr;
                }

                _tables.swap(newTables);
            }

            size_t hashi = hash(kot(data)) % _tables.size();
            // 头插
            Node* newnode = new Node(data);
            newnode->_next = _tables[hashi];
            _tables[hashi] = newnode;
            ++_size;

            // 把新插入的值，创建迭代器返回
            return make_pair(iterator(newnode, this), true);
        }


        bool Erase(const K& key)
        {
            if (_tables.size() == 0)
            {
                return false;
            }

            Hash hash;
            KeyOfT kot;

            size_t hashi = hash(key) % _tables.size();
            Node* prev = nullptr;                      
            Node* cur = _tables[hashi];

            while (cur)
            {
                if (kot(cur->_data) == key)
                {
                    if (prev == nullptr)    // 1、头删
                    {
                        _tables[hashi] = cur->_next;
                    }
                    else                  // 2、中间删
                    {
                        prev->_next = cur->_next;
                    }

                    delete cur;
                    --_size;

                    return true;
                }
                else  
                {
                    prev = cur;
                    cur = cur->_next;    
                }
            }

            return false;
        }

        //////////////////////////////////////////////// 
        // lookup
        iterator Find(const K& key)
        {
            if (_tables.size() == 0)
            {
                return end();
            }

            Hash hash;
            KeyOfT kot;

            size_t hashi = hash(key) % _tables.size();
            Node* cur = _tables[hashi];
            while (cur)
            {
                if (kot(cur->_data) == key)
                {
                    return iterator(cur, this);
                }
                cur = cur->_next;
            }
            
            // return iterator(nullptr, this);
            return end();
        }

        ///////////////////////////////////////////// 
        // capacity 
        size_t Size()const 
        {
            return _size;
        }

        bool Empty()const 
        {
            return 0 == _size;
        }

        // 表的长度
        size_t TablesSize()
        {
            return _tables.size();
        }
        
        // 桶的总数 (空或不空)
        size_t BuckCount()const 
        {
            return _tables.capacity();
        }

        // 有数据的桶的总个数
        size_t BucketNum()
        {
            size_t num = 0;
            for (size_t i = 0; i < _tables.size(); ++i)
            {
                if (_tables[i])
                {
                    ++num;
                }
            }
            return num;
        }

        // 单个桶中的数据个数
        size_t BucketSize(const K& key) // 这里的key是vector下标
        {
            size_t len = 0;
            Node* cur = _tables[key];
            while (cur)
            {
                ++len;
                cur = cur->_next;
            }
            return len;
        }

        // 最大的桶的长度
        size_t MaxBucketLenth()
        {
            size_t maxLen = 0;
            for (size_t i = 0; i < _tables.size(); ++i)
            {
                size_t len = 0;
                Node* cur = _tables[i];
                while (cur)
                {
                    ++len;
                    cur = cur->_next;
                }

                /*if (len > 0)
                 * {
                 * printf("[%d]号桶长度: %d\n", i, len);
                 * }
                  */

                if (len > maxLen)
                {
                    maxLen = len;
                }
            }
            return maxLen;
        }
    private:
        vector<Node*> _tables;
        size_t _size = 0;  // 存储的有效数据个数
    };

/*
    void TestHash1()
    {
        int a[] = { 1, 11, 4, 15, 26, 7, 44, 25, 77, 35  };
        HashTable<int, int> ht;
        for (auto e : a)
        {
            ht.Insert(make_pair(e, e));
        }

        ht.Insert(make_pair(22, 22));

        ht.Erase(44);
        ht.Erase(4);
    }
        

    void TestHash2()
    {
        string arr[] = { "apple", "banana", "watermelon", "watermelon", "banana",  "banana"  };
        
        //HashTable<string, int, HashFuncString> countHT; // 统计次数
        
        HashTable<string, int> countHT;
        for (auto& str : arr)
        {
            auto ptr = countHT.Find(str);
            if (ptr)
            {
                ptr->_kv.second++;
            }
            else 
            {
                countHT.Insert(make_pair(str, 1));
            }
        }
    }

    void TestHash3()
    {
        int n = 10000000;
        vector<int> v;
        v.reserve(n);
        srand(time(0));
        for (int i = 0; i < n; ++i)
        {
            //v.push_back(i);
            v.push_back(rand() + i);  // 重复少
            //v.push_back(rand());  // 重复多
        }
        
        // srvize_t begin1 = clock();
        HashTable<int, int> ht;
        for (auto e : v)
        {
            ht.Insert(make_pair(e, e));
        }
        // size_t end1 = clock();

        cout << "数据个数:" << ht.Size() << endl;
        cout << "表的长度:" << ht.TablesSize() << endl;
        cout << "桶的个数:" << ht.BucketNum() << endl;
        cout << "平均每个桶的长度:" << (double)ht.Size() / (double)ht.BucketNum() << endl;
        cout << "最长的桶的长度:" << ht.MaxBucketLenth() << endl;
        cout << "负载因子:" << (double)ht.Size() / (double)ht.TablesSize() << endl;

    }
    */ 
}
