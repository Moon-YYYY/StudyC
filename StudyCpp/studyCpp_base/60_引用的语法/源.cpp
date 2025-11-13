#include<iostream>
using namespace std;
//引用的语法 &
int main() {
	//引用的写法
	int a = 120;
	int& b = a;//相当于把a的名字改成b了
	b = 200;
	cout << a << ' ' << b << endl;

	//指针的写法
	int* p = &a;
	*p = 300;
	cout << a << ' ' << *p << endl;
	return 0;
}