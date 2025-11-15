#include<iostream>
using namespace std;

//1、结构体定义
//struct 结构体名 {结构体成员变量列表}
struct Book
{
	string name;
	double price;
	int value;
}cpp, pyth;//第三种定义方法
int main()
{
	Book c;
	c.name = "C语言程序设计";//第一种定义方法
	c.price = 39;
	c.value = 10;
	cout << c.name << ' ' << c.price << ' ' << c.value << endl;

	Book py = { "python编程", 1999, 10 };//第二种定义方法
	cout << py.name << ' ' << py.price << ' ' << py.value << endl;


	cpp.name = " C++零基础编程";
	cpp.price = 100000;
	cpp.value = 100000;
	cout << cpp.name << ' ' << cpp.price << ' ' << cpp.value << endl;

	pyth.name = "123445";
	cout << pyth.name << endl;
	return 0;
}
