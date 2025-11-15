#include<iostream>
using namespace std;
//继承
class Animal {
public:
	void eat() {
		cout << "吃" << endl;
	}
};
class Cat : public Animal {//继承
public:
	//void eat(){
		//cout << "吃" << endl;//和狗的吃是一样的所以可以用继承来减少冗余代码
	//}
	void say() {
		cout << "喵" << endl;
	}
};
class Dog : public Animal {
public:
	//void eat() {
		//cout << "吃" << endl;
	//}
	void say() {
		cout << "汪" << endl;
	}
};

int main() {
	Cat c;
	Dog d;
	c.eat();
	d.eat();
	c.say();
	d.say();
	return 0;
}
