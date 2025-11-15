#include<iostream>
using namespace std;
/*
1、函数名称和类名一致，并且在最前面加上~波浪号
2、函数返回值不用写
3、不能有参数
*/

class Hero
{
public:
	Hero()
	{
		cout << "Hero默认构造函数调用完毕" << endl;
	}
	//析构函数
	~Hero()
	{
		cout << "Hero析构函数调用完毕" << endl;
	}
};
void test()
{
	Hero te;
}

int main()
{
	test();
	Hero h;
	int a;
	cin >> a;

	return 0;
}
