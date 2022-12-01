
#include "mySet.h"
#include "myMap.h"
int main()
{
    try
    {
        skk::test_set1();
        skk::test_map1();
    }
    catch (const exception& e) // 抛异常
    {
        cout << e.what() << endl;
    }

    return 0;
}


