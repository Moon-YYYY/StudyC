#include<iostream>

using namespace std;
//非安全函数
#define _CRT_SECURE_NO_WARNINGS//声明，禁用安全警告，否则iostream里用到一些不安全的函数，会有编译警告
int main() {
	int a;
	a = 10;
	scanf_s("%d", &a);//使用scanf_s函数代替scanf
	printf("%d\n", a);
	char str[10];
	strcpy_s(str, "hello");
	cout << str << endl;
	return 0;
}