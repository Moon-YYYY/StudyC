#include<iostream>
#include<string>
using namespace std;
/*
静态成员变量的特点
1、所有的实例化对象共享同一份数据
2、编译阶段分配内存
3、需要在类中声明，在类外进行初始化
*/
class Hero
{
public:
	Hero()
	{
		m_Name = "英雄";
		m_Hp = 100;
	}

	static int herocount;//静态成员变量
private:
	string m_Name;
	int m_Hp;
};
int Hero::herocount = 100;//静态成员变量的初始化
int main()
{
	Hero h;
	cout << h.herocount << endl;
	h.herocount = 101;
	Hero h2;
	cout << h.herocount << endl;//和Hero::herocount是同一地址
	cout << Hero::herocount << endl;
	cout << h2.herocount << endl;//所有的实例化对象共享同一份数据
	return 0;
}
