#include "MyString.h"

int main()
{
	try
	{
		//bit::TestString1();
		bit::TestString2();
	}
	catch (const exception& e) // 抛异常处理
	{
		cout << e.what() << endl;  
	}
	
	return 0;
}
