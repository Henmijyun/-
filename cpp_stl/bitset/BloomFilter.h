#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

// 布隆过滤器
struct HashBKDR 
{
    // BKDR
    size_t operator()(const string& key)
    {
        size_t val = 0;
        for (auto ch : key)
        {
            val *= 131;
            val += ch;
        }

        return val;
    }
};

struct HashAP
{
    // AP 
    size_t operator()(const string& key)
    {
        size_t hash = 0;
        for(size_t i = 0; i < key.size(); ++i)
        {
            if ((i & 1) == 0)
            {
                hash ^= ((hash << 7) ^ key[i] ^ (hash >> 3));
            }
            else 
            {
                hash ^= (~((hash << 11) ^ key[i] ^ (hash >> 5)));
            }
        }
        return hash;
    }
};

struct HashDJB
{
    // DJB 
    size_t operator()(const string& key)
    {
        size_t hash = 5381;
        for (auto ch : key)
        {
            hash += (hash << 5) + ch;
        }
        return hash;
    }
};

// N表示准备要映射N个值
template<size_t N, 
class K = string, class Hash1 = HashBKDR, class Hash2 = HashAP, class Hash3 = HashDJB>
class BloomFilter
{
public:
    void Set(const K& key)
    {
        size_t hash1 = Hash1()(key) % (_ratio * N);
        _bits->set(hash1);
        
        size_t hash2 = Hash2()(key) % (_ratio * N);
        _bits->set(hash2);
        
        size_t hash3 = Hash3()(key) % (_ratio * N);
        _bits->set(hash3);
    }

    bool Test(const K& key)
    {
        size_t hash1 = Hash1()(key) % (_ratio * N);
        if (!_bits->test(hash1))
            return false; // 准确的 

        size_t hash2 = Hash2()(key) % (_ratio * N);
        if (!_bits->test(hash2))
            return false; // 准确的

        size_t hash3 = Hash3()(key) % (_ratio * N);
        if (!_bits->test(hash3))
            return false; // 准确的

        return true;  // 可能存在误判
    }

    //布隆过滤器不支持删除
    
private:
    const static size_t _ratio = 5;
    std::bitset<_ratio * N>* _bits = new std::bitset<_ratio * N>;
    // 库用的bitset是在栈上面开空间，数据量一大会爆，所以这里new在堆里
};

void TestBloomFilter1()
{
    srand(time(0));
    const size_t N = 100000;
    BloomFilter<N> bf;
    cout << sizeof(bf) << endl;

    std::vector<std::string> v1;
    std::string url = "https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html";
        
    for (size_t i = 0; i < N; ++i)
    {
        v1.push_back(url + std::to_string(1234 + i));
    }

    for (auto& str : v1)
    {
        bf.Set(str);                                            
    }

    // 相似
    std::vector<std::string> v2;
    for (size_t i = 0; i < N; ++i)                    
    {
        std::string url = "http://www.cnblogs.com/-clq/archive/2021/05/31/2528153.html";
        url += std::to_string(rand() + i);
        v2.push_back(url);
    }
    
    size_t n2 = 0;
    for (auto& str : v2)
    {
        if (bf.Test(str))
        {
            ++n2;
        }
    }

    cout << "相似字符串误判率:" << (double)n2 / (double)N << endl;

    std::vector<std::string> v3;
    for (size_t i = 0; i < N; ++i)
    {
        string url = "zhihu.com";
        url += std::to_string(rand()+i);
        v3.push_back(url);   
    }
    
    size_t n3 = 0;
    for (auto& str : v3)
    {
        if (bf.Test(str))
        {
            ++n3;
        }             
    }
         
    cout << "不相似字符串误判率:" << (double)n3 / (double)N << endl;
    
}

void TestBloomFilter2()
{
    BloomFilter<10> bf;
    string arr1[] = { "苹果", "西瓜", "阿里", "美团", "苹果", "字节", "西瓜", "苹果", "香蕉", "苹果", "腾讯"  };

    for (auto& str : arr1)
    {
        bf.Set(str);

    }
    
    for (auto& str : arr1)
    {
        cout << bf.Test(str) << endl;                            
    }
    
    cout << endl << endl;

    string arr2[] = { "苹果111", "西瓜", "阿里2222", "美团", "苹果dadcaddxadx", "字节", "西瓜sSSSX", "苹果 ", "香蕉", "苹果$", "腾讯"  };

    for (auto& str : arr2)
    {
        cout <<str<<":"<<bf.Test(str) << endl;                                    
    }
}


