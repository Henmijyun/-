
#include "mySet.h"
#include "myMap.h"
int main()
{
    try
    {
        cout << "---TestOfSet---" << endl;
        skk::test_set1();

        cout << endl;

        cout << "---TestOfMap---" << endl;
        skk::test_map1();
    }
    catch (const exception& e) // 抛异常
    {
        cout << e.what() << endl;
    }

    return 0;
}


