#include <iostream>
#include <string>
#include <vector> 

using namespace std;

#include "HashTable.h"
#include "myUnorderedSet.h"
#include "myUnorderedMap.h"

int main()
{
    try 
    {
        // HashBucket::TestHash3();
        
        skk::test_map1();
        skk::test_set1();
    }
    catch (const exception& e)
    {
        cout << e.what() << endl;
    }
    catch (...)
    {
        cout << "Unkown Exception" << endl;               
    }

    return 0;
}


