#include<iostream>
using namespace std;
//函数指针数组
typedef void(*fptr[4])(int a, float b, double e, float d);//第一种定义方法
typedef void(*fptrs)(int a, float b, double e, float d);//第二种定义方法

void func1(int a, float b, double e, float d)
{
	cout << "func1" << endl;
}
void func2(int a, float b, double e, float d)
{
	cout << "func2" << endl;
}
void func3(int a, float b, double e, float d)
{
	cout << "func3" << endl;
}
int main()
{
	fptr fps = { func1, func2, func3 };//第一种定义方法
	fptrs fps1[4] = { func1, func2, func3 };//第二种定义方法更好理解
	cout << fps1[0] << endl;
	cout << fps1[1] << endl;
	cout << fps1[2] << endl;
	cout << fps1[3] << endl;//定义四个元素 初始化三个 则后边没有初始化的元素为NULL

	cout << fps[0] << endl;
	cout << fps[1] << endl;
	cout << fps[2] << endl;
	cout << fps[3] << endl;//定义四个元素 但是初始化只有三个元素 则后边没有初始化的元素都为NULL

	return 0;
}
