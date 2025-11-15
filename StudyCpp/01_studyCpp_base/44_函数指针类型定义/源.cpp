#include<iostream>
using namespace std;

//函数指针的类型定义
void (*ptr)(int a, int b, int c, int d, int e);//传统的定义费时费力

typedef void(*ptr1)(int a, int b, int c, int d, int e);//自定义类型不需要重复定义只需要调用即可

void func1(int a, int b, int c, int d, int e)//参数必须和自定义的类型相同否则无法调用
{
	cout << "func1" << endl;
}
int main()
{
	ptr1 ptr = func1;
	ptr(1, 2, 3, 4, 5);
	return 0;
}
