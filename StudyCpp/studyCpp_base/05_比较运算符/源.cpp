#include<iostream>
using namespace std;
/*
比较运算符
== 判断是否相等
!= 判断是否不等
<小于
>大于
<=小于等于
>=大于等于
*/
int main() {
	int a = 9;
	int b = 9;
	int c = 8;
	cout << (a == b) << endl;//输出1，因为a和b相等
	cout << (a != b) << endl;//输出0，因为a和b相等而不是不等
	cout << (a == c) << endl;//输出0，因为a和c不相等
	cout << (a != c) << endl;//输出1，因为a和c不相等为真
}