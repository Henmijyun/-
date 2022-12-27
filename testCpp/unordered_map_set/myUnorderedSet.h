#pragma once 

#include "HashTable.h"

namespace skk
{
    template<class K, class Hash = HashFunc<K>>
    class unordered_set
    {
        struct SetKeyOfT // 内部类
        {
            const K& operator()(const K& key)
            {
                return key;            
            }
        };

    public:
        bool insert(const K& key)
        {
            return _ht.Insert(key);
        }

    private:
        HashBucket::HashTable<K, K, Hash, SetKeyOfT> _ht;
    };


    void test_set1()
    {
        unordered_set<int> s;
        s.insert(2);
        s.insert(3);
        s.insert(1);
        s.insert(5);
    }
}


