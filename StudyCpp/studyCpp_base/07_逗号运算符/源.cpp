#include<iostream>
using namespace std;
//逗号运算符 优先级很低，甚至低于等于号
int main() {
	int x = 4;
	int y = 5;
	int tmp = x;
	x = y;
	y = tmp;
	cout << x << ' ' << y << endl;//此时x = 5， y = 4，两个变量交换了值
	//通过以下代码展示逗号运算符
	tmp = x, x = y, y = tmp;//利用逗号运算符较低优先级的特性使代码简洁
	cout << x << ' ' << y << endl;//此时x = 4， y = 5，两个变量又交换了值
	return 0;
}