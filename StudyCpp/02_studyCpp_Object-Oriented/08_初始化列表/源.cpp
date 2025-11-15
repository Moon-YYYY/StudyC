#include<iostream>
#include<string>
using namespace std;

/*
初始化列表的语法
构造函数（）：成员变量（传参），成员变量（传参）
{
	...
}

*/
class Hero
{
public:
	/*Hero(string name, int hp)
	{
		m_Name = name;
		m_Hp = hp;
	}*/
	Hero(string name, int hp) :m_Name("猴子"), m_Hp(50)//要想变成剑圣和血量100，就把猴子改成name，50改成hp
	{

	}
	void Print()
	{
		cout << "英雄：" << m_Name << "的血量是" << m_Hp << endl;
	}
private:
	string m_Name;
	int m_Hp;
};
int main()
{
	Hero h("剑圣", 100);
	h.Print();
	return 0;
}
