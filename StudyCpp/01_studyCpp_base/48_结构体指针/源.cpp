#include<iostream>
#include<string>
using namespace std;
//结构体指针
struct Book
{
	string name;
	double price;
	int value;
};
int main()
{
	Book b = { "C语言", 1999, 10 };
	Book c = b;
	c.name = { "c++" };//此举不会改变b里面的值
	cout << b.name << ' ' << b.price << ' ' << b.value << endl;
	//要想改变b里面的值可以通过结构体指针来实现
	Book* pb = &b;
	pb->name = "C++";//此举可以改变b里面的值
	cout << b.name << ' ' << b.price << ' ' << b.value << endl;
	return 0;
}
