#include<iostream>
using namespace std;
//函数调用运算符重载
class addFunctor {
public:
	addFunctor() {
		m_acc = 0;
	}
	int operator()(int a, int b) {
		m_acc++;
		return a + b + m_acc;
	}
private:
	int m_acc;
};
int Add(int a, int b) {
	return a + b;
}
int main() {
	addFunctor add;
	cout << add(5, 6) << endl;//仿函数
	cout << add(5, 6) << endl; cout << add(5, 6) << endl;
	cout << Add(5, 6) << endl;
	return 0;
}
