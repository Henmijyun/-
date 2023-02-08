#include "myBitset.h"
#include "BloomFilter.h"

int main()
{
    try 
    {
    skk::test_bitset1();
    skk::test_bitset3();

    cout << endl;
    TestBloomFilter1(); 
    TestBloomFilter2(); 
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





