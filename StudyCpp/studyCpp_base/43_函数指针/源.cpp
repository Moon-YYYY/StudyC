#include<iostream>
using namespace std;
//函数指针
double (*ptr)(int a, int b, int c);
void (*ptr1)(int a, int b);
double func1(int a, int b, int c) {
	cout << a << ' ' << b << ' ' << c << endl;
	return 0.0;
}
void func2(int a, int b) {
	cout << a << ' ' << b << endl;
}
int main() {
	ptr = func1;
	ptr(1, 2, 3);//通过调用函数指针调用函数
	ptr1 = func2;
	ptr1(4, 5);
	return 0;
}