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

        bool insert(const pair<K, V>& kv)
        {
            return _t.Insert(kv);
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
    }
}

