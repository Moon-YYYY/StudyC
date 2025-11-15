#include<iostream>
using namespace std;
//const修饰成员函数
//常函数
class Hero
{
public:
	Hero() :m_Hp(0)
	{

	}
	int getHp()const //此举可以让函数内部的成员变量不可修改
	{
		//m_Hp = m_Hp + 1;//错误，因为已经声明了const，函数内不能修改了
		return m_Hp;
	}
private:
	int m_Hp;
};

int main()
{
	Hero h;
	const Hero h1;//const修饰成员函数第二种方法
	h.getHp();
	cout << h.getHp() << endl;//如果没有const,输出为2
	return 0;
}
