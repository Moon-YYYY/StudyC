//1、普通函数有隐式类型转换
//2、函数模板自动类型推导无隐式类型转换
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
	double d = 0.9;
	//int c = add(a, d);//函数模板自动类型推导无隐式类型转换,参数T不可能既是int又是double
	return 0;
}
//3、函数模板显式指定类型有隐式类型转换
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
	double d = 0.9;
	int c = add<int>(a, d);//函数模板显式指定类型有隐式类型转换，d从double转换到了int
	return 0;
}
