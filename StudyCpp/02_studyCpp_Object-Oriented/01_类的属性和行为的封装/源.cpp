#include<iostream>
using namespace std;
/*
面向对象三大特性：封装，继承，多态

封装的语法：
class 类名
{
	访问权限：
	属性（成员变量）
	行为（成员函数）
};//类的结尾必须要有分号

*/

class hero
{
	//访问权限有三种：public, private, protected
public:
	//属性
	int m_Id;
	int m_Hp;
	//行为
	void addHp(int hp)
	{
		m_Hp += hp;
	}
	void subHp(int hp)
	{
		m_Hp -= hp;
	}
};

int main()
{
	//通过类来生成对象的过程叫实例化
	hero h;
	h.m_Id = 5;
	h.m_Hp = 100;
	h.addHp(100);
	cout << "Id为" << h.m_Id << "的血量是" << h.m_Hp << endl;
	return 0;
}
