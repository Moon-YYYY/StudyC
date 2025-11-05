#include<iostream>
using namespace std;
// %取余符号  其正负性与被除数有关.
int main() {
	int a = -9;
	int b = 2;
	cout << a % b << endl;//输出-1

	int c = 9;
	int d = -2;
	cout << c % d << endl;//输出为1

	//取余符号还适用于判断数字的奇偶性
	int e = 9, f = 10;
	cout << e % 2 << endl;//输出1，奇数%2输出1
	cout << f % 2 << endl;//输出0，偶数%2输出0

	//取余符号还适用于取一个数的最低位
	int number = 123456;
	cout << number % 10 << endl;//输出6

	return 0;
}