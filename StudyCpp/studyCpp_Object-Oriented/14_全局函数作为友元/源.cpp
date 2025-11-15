#include<iostream>
#include<string>
using namespace std;
/*
友元的目的
让一个类或者函数能够访问另一个类的私有成员

友元的关键字： friend

三种友元：
1、全局函数作为友元
2、类作为友元
3、成员函数作为友元
*/
class people
{
	friend void friendVisit(people* p);//在类内声明一次就可以访问类内的私有成员
public:
	people()
	{
		m_House = "别墅";
		m_Car = "跑车";
		m_name = "于洋";
	}
public:
	string m_House;
private:
	string m_Car;
protected:
	string m_name;
};
void friendVisit(people* p)//此处使用指针是为了防止拷贝的性能开销，使用指针较少性能损耗
{
	cout << "好朋友访问你的" << p->m_House << endl;
	cout << "好朋友访问你的" << p->m_Car << endl;//如果不声明为友元函数，则无法访问其私有成员
	cout << "好朋友叫你的名字" << p->m_name << endl;//如果不声明为友元函数，则无法访问其保护成员
}
int main()
{
	people p;
	friendVisit(&p);
	return 0;
}