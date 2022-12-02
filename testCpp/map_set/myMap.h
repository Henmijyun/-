#pragma once 

#include "RedBlackTree.h"

namespace skk
{
    template<class K, class V>
    class map
    {
        struct MapKeyOfT
        {
            const K& operator()(const pair<K, V>& kv)
            {
                return kv.first;
            }
        };
    public:
        typedef typename RBTree<K, pair<K, V>, MapKeyOfT>::iterator iterator;  // typename告诉编译器这是个类型 

        iterator begin()
        {
            return _t.begin();
        }

        iterator end()
        {
            return _t.end();
        }

        pair<iterator, bool> insert(const pair<K, V>& kv)
        {
            return _t.Insert(kv);
        }

        V& operator[](const K& key)
        {
            pair<iterator,bool> ret = insert(make_pair(key, V()));
            // 原本有，返回迭代器；原本没有，插入缺省值，返回迭代器
            return ret.first->second;
        }
    private:
        RBTree<K, pair<K, V>, MapKeyOfT> _t;
    };



    // Test
    void test_map1()
    {
        map<int, int> m;
        m.insert(make_pair(1, 2));
        m.insert(make_pair(5, 2));
        m.insert(make_pair(2, 2));

        map<int, int>::iterator it = m.begin();
        while (it != m.end())
        {
            cout << (*it).first << " ";
            ++it;
        }
        cout << endl;

        string arr[] = {"apple", "banana", "watermelon", "apple", "banana", "apple"};
        map<string, int> countMap;
        for (auto& str : arr)
        {
            // 1、str不在countMap中，插入pair(str, int())，然后再对返回v次数++
            // 2、str在countMap中，返回value(次数)的引用，v次数++
            countMap[str]++;
        }

        map<string, int>::iterator it2 = countMap.begin();
        while (it2 != countMap.end())
        {
            cout << it2->first << ":" << it2->second << endl;
            ++it2;
        }
        
        for (auto& kv : countMap)
        {
            cout << kv.first << ":" << kv.second << endl;
        }
    }
}

