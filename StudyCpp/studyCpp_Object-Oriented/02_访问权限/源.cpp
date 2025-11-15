#include<iostream>
using namespace std;
/*
访问权限
公共权限 public			类内可以访问，类外可以访问
保护权限 protected		类内可以访问，类外不可访问		子类可以访问
私有权限 private		    类内可以访问，类外不可访问		子类不可以访问
B -> A
A 父类、 基类		名字、  房子、 支付密码
B 子类、 派生类		公共、  保护、 私有
*/

class people
{
public:
	int m_Id;
protected:
	int m_HouseId;
private:
	int m_PayPass;

public:
	void work()
	{
		m_Id = 1;
		m_HouseId = 2;
		m_PayPass = 12;
		//所有成员变量类内均可以访问
	}
private:
	void work1()
	{
		m_Id = 1;
		m_HouseId = 2;
		m_PayPass = 12;
		//所有成员变量类内均可以访问
	}
private:

};

class son : public people//公有继承
{
	void func1()
	{
		m_Id = 1;//公有权限，子类可以访问，类外可以访问
		m_HouseId = 10;//保护成员子类可以访问，类外不可访问
		//m_PayPass = 1;//私有成员子类无法访问
	}
};

int main()
{
	people p;
	p.m_Id = 1;//公有成员，类外可以访问
	//p.m_HouseId = 5;//保护成员类外不可以访问
	//p.m_PayPass = 10;//私有成员类外不可以访问
	//p.work1();//私有成员函数，类外不可访问
	return 0;
}
