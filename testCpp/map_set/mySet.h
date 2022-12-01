#pragma once 

#include "RedBlackTree.h"

namespace skk
{
    template<class K>
    class set
    {
        struct SetKeyOfT
        {
            const K& operator()(const K& key)
            {
                return key;
            }
        };

    public:
        typedef typename RBTree<K, K, SetKeyOfT>::iterator iterator;  // typename 告诉编译器这是个类型 
      iterator begin()
      {
          return _t.begin();      
      }

      iterator end()
      {      
          return _t.end();
      }


        bool insert(const K& key)
        {
            return _t.Insert(key);
        }
    private:
        RBTree<K, K, SetKeyOfT> _t;
    };



    // Test
    void test_set1()
    {
        set<int> s;
        s.insert(3);
        s.insert(1);
        s.insert(2);
        s.insert(5);

        set<int>::iterator it = s.begin();
        while (it != s.end())
        {
            cout << *it << " ";
            ++it;
        }
        cout << endl;
    }
}


