#include<iostream>
using namespace std;
struct Book
{
	string name;
	double price;
	int value;
};
int main()
{
	Book book[3] =
	{
		{"C语言程序设计", 1999, 10},
		{"python编程", 1999, 10},
		{"C++程序设计",100000, 100}
	};
	book[2].name = "...";//增删改查
	for (int i = 0; i < 3; ++i)
	{
		cout << book[i].name << ' ' << book[i].price << ' ' << book[i].value << endl;
	}
	return 0;
}
