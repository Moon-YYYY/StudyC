#include<iostream>
#include<string>
using namespace std;

//类作为友元
//让一个类访问另一个类的私有成员
class People;
class Peoplefriend
{
public:
	Peoplefriend() {}
	void visit(People* p);//变成函数声明
};

class People
{
	friend class Peoplefriend;//重点
public:
	People()
	{
		m_House = "别墅";
		m_Car = "跑车";
	}
public:
	string m_House;
private:
	string m_Car;
};

void Peoplefriend::visit(People* p)//写出作用域，写在类内的声明是friend void 函数名();
{
	cout << "好朋友来访问你的" << p->m_House << endl;
	cout << "好朋友来访问你的" << p->m_Car << endl;
}
int main()
{
	People p;
	Peoplefriend pf;
	pf.visit(&p);
	return 0;
}
