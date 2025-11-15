#include<iostream>
using namespace std;
/*
拷贝构造函数定义
类名（const 类型& 变量名）
{
	...
}
*/

class Hero
{
public:
	//默认构造函数
	Hero()
	{
		m_Hp = 100;
		cout << "默认构造函数调用完毕" << endl;
	}
	//有参构造函数
	Hero(int hp)
	{
		m_Hp = hp;
		cout << "有参构造函数调用完毕" << endl;
	}
	//拷贝构造函数
	Hero(const Hero& h)
	{
		m_Hp = h.m_Hp;
		cout << "拷贝构造函数调用完毕" << endl;
	}
	//析构函数
	~Hero()
	{
		cout << "析构函数调用完毕" << endl;
	}
private:
	int m_Hp;

};
/*
拷贝函数调用时机：
1、用已经创建的对象来初始化对象
	例如：
	Hero h1(20);
	Hero h2(h1);
2、函数的传参
3、函数的返回值
*/
//1、用已经创建的对象来初始化对象
void func1()//调用h1传到Hero(int hp)，因为h1里面有个20作为参数，调用h2（h1），
//传到拷贝构造函数，因为h2里面传过来了一个对象
{
	cout << "------------func1-----------" << endl;
	Hero h1(20);//调用有参构造函数
	Hero h2(h1);//调用拷贝构造函数
}
void test1(Hero h)
{

}
void test2(Hero* h)
{
	cout << "调用test2拷贝构造函数" << endl;
}
//2、函数的传参
void func2()
{
	cout << "------------func2-----------" << endl;
	Hero h1;//调用默认构造函数
	//test1(h1);//调用拷贝构造函数
	test2(&h1);
}
//3、函数的返回值
Hero test3()
{
	Hero h(40);
	return h;//调用拷贝函数

}
void func3()
{
	cout << "--------func3-------" << endl;

	Hero h = test3();
}
int main()
{
	func1();
	func2();
	func3();
	return 0;
}
