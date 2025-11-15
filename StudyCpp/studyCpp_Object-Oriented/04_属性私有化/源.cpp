#include<iostream>
#include<string>
using namespace std;
//属性私有化
//接口，方法，函数是同一个概念
//可以控制读写权限， 可以检测数据的有效性
class hero
{
public:
	void SetName(string name)
	{
		m_Name = name;
	}
	string GetName()
	{
		return m_Name;
	}

	int GetSkillcount()
	{
		return m_Skillcount;//只能读取不能修改
	}
	void Speed(int speed)
	{
		if (speed > 500 || speed < 100)
		{
			cout << "速度设置不合法" << endl;
			return;
		}
		m_Speed = speed;
	}
private:
	string m_Name;			//可读可写
	int m_Skillcount = 4;	//只读
	int m_Speed;			//只写

};
int main()
{
	hero h;
	/*
	h.m_Name;
	h.m_Skillcount;
	h.m_speed;
	*/
	h.SetName("剑圣");
	cout << "英雄的名字是" << h.GetName() << endl;
	cout << "英雄的技能数量是" << h.GetSkillcount() << endl;
	h.Speed(666);
	return 0;
}
