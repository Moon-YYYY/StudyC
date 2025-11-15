#include<iostream>
using namespace std;
//同名属性
class Animal {
public:
	Animal() {
		m_Data = 1;
	}
	int m_Data;
};
class cat :public Animal {
public:
	cat() {
	m_Data = 2;
}
	  int m_Data;
};
void test() {
	cat c;
	cout << c.m_Data << endl;
	cout << c.Animal::m_Data << endl;
	cout << &c.m_Data << endl;//地址不在同一地方
	cout << &c.Animal::m_Data << endl;//通过此代码来访问父类的同名变量
}
int main() {

	test();
	return 0;
}
