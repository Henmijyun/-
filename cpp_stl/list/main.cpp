
#include "myList.h"


void test_list1()
{
    list<int> lt;
    lt.push_back(1);
    lt.push_back(2);
    lt.push_back(3);
    lt.push_back(4);
    lt.push_back(5);

    list<int>::iterator it = lt.begin();
    while (it != lt.end())
    {
        cout << *it << " ";
        ++it;
    }
    cout << endl;

    for (auto e : lt)
    {
        e *= 2;
        cout << e << " ";
    }
    cout << endl;
}

void test_list2()
{
    list<int> lt;
    lt.push_back(1);
    lt.push_back(2);
    lt.push_back(3);
    lt.push_back(4);
    lt.push_back(5);

    auto pos = find(lt.begin(), lt.end(), 3);
    if (pos != lt.end())
    { 
        // 这里pos不会迭代器失效，pos指向不会改变，可以重复插入和修改
        lt.insert(pos, 30); // 在pos位置前面插入一个节点30
        *pos *= 100;
    }

    for (auto e : lt)
    {
        cout << e << " ";
    }
    cout << endl;

    pos = find(lt.begin(), lt.end(), 300);
    if (pos != lt.end())
    {
        // 这里pos用完一次之后，迭代器会失效
        lt.erase(pos); // 删除节点，并释放空间
    }

    for (auto e : lt)
    {
        cout << e << " ";
    }
    cout << endl;
}


int main()
{
    try
    {
       // test_list2();
       // skk::test_list1();
       skk::test_list3();
    }
    catch (const exception& e) // 抛异常
    {
        cout << e.what() << endl;
    }

    return 0;
}
    













