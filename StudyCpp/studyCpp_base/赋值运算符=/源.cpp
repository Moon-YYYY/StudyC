#include<iostream>
using namespace std;
// = 赋值运算符
int main() {
	int a = 10;
	int b = a;
	cout << b << endl;//将a赋值给b，输出为10
	b += 2;//等同于 b = b + 2;同理，b -= 2; b *= 2; b /= 2;同样适用,与==不同 =是赋值运算符，==是比较运算符
	return 0;
}