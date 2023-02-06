#include "myString.h"

int main()
{
	try
	{
		//skk::TestString1();
		skk::TestString2();
	}
	catch (const exception& e) // 抛异常处理
	{
		cout << e.what() << endl;  
	}
	
	return 0;
}
