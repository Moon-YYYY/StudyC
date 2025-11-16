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
	double f = 3.14;
	int c = add<int>(a, f);//显式指定类型调用，指定T类型为int
	cout << c << endl;//输出12，f为double，转为了int小数的数据丢失，9+3=12
	double d = 1.4, e = 2.5;
	cout << add<int>(d, e) << endl;//double转到int会丢失数据保留整数
	return 0;
}
