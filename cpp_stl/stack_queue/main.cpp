

#include "myStack.h"
#include "myQueue.h"
#include "myPriorityQueue.h"

int main()
{
    try
    {
        skk::test_stack1();
        skk::test_queue1();
        skk::test_priority_queue1();

    }
    catch (const exception& e) // 抛异常
    {
        cout << e.what() << endl;
    }
    catch (...)
    {
        cout << "Unkown Exception" << endl;                
    }

    return 0;
}


