#pragma once 

#include "HashTable.h"

namespace skk
{
    template<class K, class V, class Hash = HashFunc<K>>
    class unordered_map
    {
        struct MapKeyOfT  // 内部类
        {
            const K& operator()(const pair<K, V>& kv)
            {
                return kv.first;
            }
        };

    public:
        typedef typename HashBucket::HashTable<K, pair<K, V>, Hash, MapKeyOfT>::iterator iterator;  

        unordered_map()
            : _ht()
        {}

        ////////////////////////////////////////// 
        // iterator 
        iterator begin()  
        {  
            return _ht.begin();  
        }  
        
        iterator end()  
        {  
            return _ht.end();
        }

        ////////////////////////////////////////////// 
        // capacity 
        size_t size()const 
        {
            return _ht.Size();
        }

        bool empty()const 
        {
            return _ht.Empty();
        }

        /////////////////////////////////////////// 
        // Acess 
        V& operator[](const K& key)
        {
            pair<iterator, bool> ret = _ht.Insert(make_pair(key, V()));
            return ret.first->second;  // ret.first是迭代器
        }
        
        const V& operator[](const K& key)const; 

        //////////////////////////////////////////////// 
        // lookup
        iterator find(const K& key)
        {
            return _ht.Find(key);
        }

        size_t count(const K& key)
        {
            return _ht.Count(key);
        }

        ///////////////////////////////////////////////// 
        // modify 
        pair<iterator, bool> insert(const pair<K, V>& kv)
        {
            return _ht.Insert(kv);
        }

        iterator erase(iterator pos)
        {
            return _ht.Erase(pos);
        }

        ////////////////////////////////////////////////// 
        // bucket 
        size_t bucket_count()
        {
            return _ht.BucketCount();
        }

        size_t bucket_size(const K& key) 
        {
            return _ht.BucketSize(key);
        }
    private:
        HashBucket::HashTable<K, pair<K, V>, Hash, MapKeyOfT> _ht;
    };


    void test_map1()
    {
        unordered_map<string, string> dict;
        dict.insert(make_pair("sort", "排序"));
        dict.insert(make_pair("string", "字符串"));
        dict.insert(make_pair("left", "左边"));

        unordered_map<string, string>::iterator it = dict.begin();
        while (it != dict.end())
        {
            cout << it->first << ":" << it->second << endl;
            ++it;
        }
        cout << endl;

        unordered_map<string, int> countMap;
        string arr[] = { "apple", "banana", "watermelon", "watermelon", "banana",  "banana"  };
        for (auto e : arr)
        {
            countMap[e]++; 
            // 通过字符串查找桶的位置：
            // 该位置没有数据的话，插入数据，然后second的int++。
            // 该位置已存在数据的话，直接second的int++
        }

        for (auto& kv : countMap)
        {
            cout << kv.first << ":" << kv.second << endl;
            // 通过字母找到位置，
        }
    }
}


