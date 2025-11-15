#include <iostream>
using namespace std;
//函数
/*
1、返回值类型
2、函数名
3、参数列表
4、函数体
5、return表达式
*/
/*
返回值类型 函数名 (参数列表) {
	函数体
	return 返回表达式;
}
*/
int add(int a, int b) {//a和b是形参
	int c = a + b;
	a++;
	b++;//形参的改变不会改变实参10，20的值
	return c;
}
int main() {
	int sum = add(10, 20);//此处的10和20是实参，传入add函数作为形参
	cout << "10 + 20 = " << sum << endl;//输出10 + 20 = 30
	return 0;
}