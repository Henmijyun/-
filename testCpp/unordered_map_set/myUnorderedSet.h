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
        typedef typename HashBucket::HashTable<K, K, Hash, SetKeyOfT>::iterator iterator;

        iterator begin()    
        {    
            return _ht.begin();  
        }  
                 
        iterator end()  
        {  
            return _ht.end();                                                              
        }                

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

        unordered_set<int>::iterator it = s.begin();
        while (it != s.end())
        {
            cout << *it << " "; 
            ++it;
        }
        cout << endl;
    }
}


