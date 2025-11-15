#include <iostream>
using namespace std;
//指针引用

void func1(char*&/*&引用使P和p变成同一个char，所以给P分配内存就实现了给p分配内存*/ P, int c)
{
	P = new char[c];
}
void list()
{
	char* p = NULL;
	func1(p, 5);
	cout << (int*)p << endl;
}
int main()
{
	list();
	return 0;
}
