#include<iostream>
using namespace std;
//构造和析构函数调用顺序
class Animal {
public:
	Animal() {
		cout << "Animal 构造" << endl;
	}
	~Animal() {
		cout << "Animal 析构" << endl;
	}
};
class cat : public Animal {
public:
	cat() {
		cout << "cat 构造" << endl;
	}
	~cat() {
		cout << "cat 析构" << endl;
	}
};
class bosscat : public cat {
public:
	bosscat() {
		cout << "bosscat 构造" << endl;
	}
	~bosscat() {
		cout << "bosscat 析构" << endl;
	}
};
void test(){
	//Animal a;
	//cat c;
	bosscat b;//会先调用Animal类里面的构造函数，然后调用父类cat里面的构造函数，然后调用子类bossct里面的构造函数
	//然后调用子类bosscat的析构函数，再调用父类cat的析构函数，再调用animal的析构函数
	//很像栈的先进后出的特性，继承中，构造链里，先构造的后析构
	//继承：d->c->b->a
	//构造和析构：a->b->c->d->d->c->b->a
}
int main(){
	test();
	return 0;
}
