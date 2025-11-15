#include<iostream>
using namespace std;
struct dataS
{
	int i;
	double d;
	char s[10];
};
union dataU//优点在于允许不同成员使用同一块内存空间，节省内存，但是只能同时使用其中一个成员
{
	int i;
	double d;
	char s[10];
};
/*
1、定义和使用分开
union dataU
{
	int i;
	double d;
	char s[10];
};
dataU a, b, c;
2、定义和使用结合
union dataU
{
	int i;
	double d;
	char s[10];
}a, b, c;
3、匿名：不想让别人使用
union
{
	int i;
	double d;
	char s[10];
}a, b, c;
*/
int main()
{
	dataS ds;
	dataU du;
	cout << &ds.i << "," << &ds.d << "," << &ds.s << endl;//不同地址
	cout << &du.i << "," << &du.d << "," << &du.s << endl;//使用同一起始地址
	du.i = 10;
	cout << du.i << endl;
	du.d = 3.14;
	cout << du.d << endl;
	cout << du.i << endl;//使用了du.d之后，du.i数据会丢失，因为只能同一时间使用一个成员
	return 0;
}
