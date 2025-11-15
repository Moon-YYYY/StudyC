#include<iostream>
using namespace std;
/*
纯虚函数和抽象类
*/
class Animal {
public:
	virtual void eat() = 0;//= 0 变成纯虚函数，只为了给子类继承
};
class cat :public Animal {
public:
	virtual void eat() { //重写纯虚函数
		cout << "猫在吃东西" << endl;
	}
};
int main()
{
	cat a;
	a.eat();
	return 0;
}
