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

        unordered_set()
            : _ht()
        {}

        ////////////////////////////////////////  
        // iterator 
        iterator begin()    
        {    
            return _ht.begin();  
        }  
                 
        iterator end()  
        {  
            return _ht.end();                                                              
        }                
    
        ////////////////////////////////////// 
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
        // modify 
        pair<iterator, bool> insert(const K& key)
        {
            return _ht.Insert(key);
        }

        iterator erase(iterator pos)
        {
            return _ht.Erase(pos);
        }

        //////////////////////////////////////// 
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


