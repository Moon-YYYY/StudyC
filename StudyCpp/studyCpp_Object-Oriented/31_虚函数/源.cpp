#include<iostream>
using namespace std;
//虚函数
class Animal {
public:
	virtual void eat() {//子类会覆盖掉父类的虚函数数组的首地址
		cout << "动物在吃东西" << endl;
	}
	virtual void run() {
		cout << "动物再跑" << endl;
	}
};
class cat :public Animal {
public:
	void eat() {
		cout << "猫在吃东西" << endl;
	}
};
void eat(Animal& a) {
	Animal b;
	a.eat();
}
void test() {
	cat c;
	eat(c);
	cout << "Animal's size : " << sizeof(Animal) << endl;//1字节
}
int main() {
	test();
	return 0;
}
