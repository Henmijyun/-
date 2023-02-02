
#include "myVector.h"

int main()
{
    try
    {
       // skk::TestVector1();
       // skk::TestVector2();
       // skk::TestVector3();
       skk::TestVector4();
    }
    catch (const exception& e) // 抛异常处理
    {
        cout << e.what() << endl;
    }
    catch (...)
    {
        cout << "Unkown Exception" << endl;
    }

    return 0;
}



