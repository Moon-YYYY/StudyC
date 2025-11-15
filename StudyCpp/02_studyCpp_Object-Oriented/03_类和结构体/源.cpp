#include<iostream>
using namespace std;

/*类和结构体
struct 和 class
struct  默认公开的
class   默认私有的
*/

class C
{
	int m_a;
};
struct S
{
	int m_a;
	void func1()
	{
		m_a = 666;
	}
};
int main()
{
	C c;
	S s;
	//c.m_a;	//默认私有的
	s.m_a = 4;	//默认公有的
	s.func1();  //C++可以定义成员函数，但C语言里不可以
	cout << s.m_a << endl;
	return 0;
}
