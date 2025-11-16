#include<iostream>
using namespace std;
/*
函数模板
template<typename T>
函数的定义
*/
template<typename T>//和传统的函数不同，此函数可以不定义传入参数类型，而是根据传入参数类型推导
T add(T a, T b) {
	return  a + b;
}
int main()
{
	int a = 9, b = 8;
	cout << add(a, b) << endl;//传入的a和b必须是同一类型
	double c = 3.14, d = 3.15;
	cout << add(c, d) << endl;//函数模板的作用之一，调用同样的函数但是可以传入不同的参数类型，但是传入的两个参数的类型必须相同
	return 0;
}
