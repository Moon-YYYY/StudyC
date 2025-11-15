#include<iostream>
using namespace std;

/*
mutable <-> const  和const相对

*/
class Hero
{
public:
	Hero() :m_Hp(0), m_getHpCounter(0)
	{

	}
	int GetHp()const
	{
		m_getHpCounter++;
		return m_Hp;
	}
	void printcounter()
	{
		cout << "counter:" << m_getHpCounter << endl;
	}
private:
	int m_Hp;
	mutable int m_getHpCounter;//mutable关键字表示可修改不写mutable则GetHp函数会报错，因为GetHp是常函数
};

int main()
{
	Hero h;
	h.GetHp(), h.GetHp(), h.GetHp(), h.GetHp(), h.GetHp(), h.GetHp();//调用六次
	h.printcounter();
	return 0;
}
