#include<iostream>
#include<string>
using namespace std;
//构造函数的作用是用来初始化类的对象，在创建对象时自动调用。
/*
构造函数需要注意
1、函数名称和类的名称一致
2、返回值类型不需要写
3、构造函数可以有参数
*/
class Hero
{
public:
	//构造函数
	Hero()
	{
		m_SkillCount = 4;
		m_Speed = 100;
		m_Name = "";
		cout << "默认Hero构造完毕" << endl;
	}
	//有参构造函数
	Hero(string Name, int skillcount)
	{
		m_SkillCount = skillcount;
		m_Speed = 100;
		m_Name = Name;
		cout << "有参Hero构造完毕" << endl;
	}
private:
	string m_Name;
	int m_SkillCount;
	int m_Speed;
};
int main()
{

	Hero h;					//调用默认构造函数
	Hero h1("剑圣", 4);		//调用有参构造函数
	Hero h2();				//函数声明， int main() int work()
	Hero h3{};				//默认构造函数
	Hero h5 = Hero("剑圣", 4);	//调用有参构造函数
	return 0;
}
