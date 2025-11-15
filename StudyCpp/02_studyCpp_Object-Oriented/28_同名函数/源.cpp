#include<iostream>
using namespace std;

class Animal {
public:
	Animal() {}
	void eat() {
		cout << "动物吃东西" << endl;
	}
};

class cat : public Animal {
public:
	cat() {}
	void eat(){
		Animal::eat();//也可以通过此方式调用父类同名函数
		cout << "猫吃东西" << endl;
	}
};

int main(){
	cat c;
	c.eat();//调用猫吃东西
	//c.Animal::eat();//调用动物吃东西
	return 0;
}
