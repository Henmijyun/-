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
        bool Insert(const pair<K, V>& kv)
        {
            return _ht.Insert(kv);
        }
    private:
        HashBucket::HashTable<K, pair<K, V>, Hash, MapKeyOfT> _ht;
    };


    void test_map1()
    {
        unordered_map<string, string> dict;
        dict.Insert(make_pair("sort", "排序"));
        dict.Insert(make_pair("string", "字符串"));
        dict.Insert(make_pair("left", "左边"));
    }
}


