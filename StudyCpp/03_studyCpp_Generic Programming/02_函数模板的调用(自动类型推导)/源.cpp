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
	return 0;
}
