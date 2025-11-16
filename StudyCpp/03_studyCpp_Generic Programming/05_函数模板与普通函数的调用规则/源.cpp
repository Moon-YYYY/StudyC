//1、优先调用普通函数
#include<iostream>
using namespace std;
/*
函数模板
template<typename T>
函数的定义
*/
template<typename T>
T add(T a, T b) {
	cout << "调用函数模板" << endl;
	return  a + b;
}
int add(int a, int b)
{
	cout << "调用普通函数" << endl;
	return  a + b;
}
int main()
{
	int a = 9, b = 8;
	double d = 0.9;
	int c = add(a, d);//当函数模板名和普通函数名相同，此处会优先调用普通函数
	return 0;
}
//2、强制调用函数模板
#include<iostream>
using namespace std;
/*
函数模板
template<typename T>
函数的定义
*/
template<typename T>
T add(T a, T b) {
	cout << "调用函数模板" << endl;
	return  a + b;
}
int add(int a, int b)
{
	cout << "调用普通函数" << endl;
	return  a + b;
}
int main()
{
	int a = 9, b = 8;
	double d = 0.9;
	int c = add<int>(a, d);//当普通函数和函数模板同名时，加上模板函数的<int>就可以强制调用函数模板，或者只写<>不加类型，效果一样
	return 0;
}

//3、函数模板优先匹配
#include<iostream>
using namespace std;
/*
函数模板
template<typename T>
函数的定义
*/
template<typename T>
T add(T a, T b) {
	cout << "调用函数模板" << endl;
	return  a + b;
}
int add(int a, int b)
{
	cout << "调用普通函数" << endl;
	return  a + b;
}
int main()
{
	double a = 9, b = 8;
	double d = 0.9;
	int c = add(a, d);//未指定显式类型也可以调用到函数模板，因为系统会优先匹配最合适的函数，普通函数的类型为int匹配不够完美，所以没调用普通函数，编译器就会调用函数模板
	return 0;
}
