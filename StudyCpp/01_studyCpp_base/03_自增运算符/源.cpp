#include<iostream>
using namespace std;
//++自增运算符
int main() {
	int a = 8;
	++a;
	cout << a << endl;//输出9
	a++;
	cout << a << endl;//输出10

	cout << a++ << endl;//输出10，因为自增运算符放在后边会导致程序先输出再自增
	cout << ++a << endl;//输出12，因为上一句执行完毕后变成a=11，此处自增运算符在前先自增再输出

	//++a比a++的效率高一点，所以for循环中用++i更好
}