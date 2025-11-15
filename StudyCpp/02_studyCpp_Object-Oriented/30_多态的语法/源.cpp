#include<iostream>
using namespace std;
//多态的语法
class Animal {
public:
	virtual/*不加virtual的话eat(c)和eat(p)会调用动物在吃东西*/ void eat()/*虚函数, 函数传参是个动物但是传入不同的动物会产生不同的行为，叫做多态*/ {
		cout << "动物在吃东西" << endl;
	}
};
class cat : public Animal {
public:
	void eat(){
		cout << "猫在吃东西" << endl;
	}
};
class pig : public Animal {
public:
	void eat(){
		cout << "猪在吃东西" << endl;
	}
};
void eat(Animal& a) {//隐式转换为Animal类型，通过虚函数达到想输出猫在吃东西的目的
	a.eat();
}
void test() {
	cat c;
	pig p;

	eat(c);
	eat(p);
}
int main() {
	test();
	return 0;
}
