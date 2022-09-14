
#include <iostream>
using namespace std;

#include <string>
#include <list>
#include <stddef.h>

#include <assert.h>

#include "MyString.h"


int main()
{
    string s1("hello");
    string s2;

    cout << s1.c_str() <<endl;  // c_str()返回c形式的字符串char*
    cout << s2.c_str() <<endl;  

    for (auto ch : s1)
    {
        cout << ch << ' ';
    }
    cout << endl;
    return 0;
}


