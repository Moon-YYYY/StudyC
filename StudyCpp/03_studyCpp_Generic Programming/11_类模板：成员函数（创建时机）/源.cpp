#include<iostream>
using namespace std;
//类模板：成员函数（创建时机）
class Player {
public:
	void run() {
		cout << "跑步" << endl;
	}
};
class Ball {
public:
	void drop() {
		cout << "下落" << endl;
	}
};

template<class T>
class Test {
	T obj;
public:
	void move1() {
		obj.run();
	}
	void move2() {
		obj.drop();
	}
};
int main() {
	Test <Player>test1;
	test1.move1();
	//test1.move2();//错误，会显示drop不是player成员

	Test<Ball>test2;
	//test2.move1();//错误，run不是Ball的成员,说明类模板的成员函数没有调用的时候没有生成出来，如果不调用就不会出错
	test2.move2();
	return 0;
}
