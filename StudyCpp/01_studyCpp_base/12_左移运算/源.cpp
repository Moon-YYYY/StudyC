#include<iostream>
using namespace std;
//左移运算 " << "
int main() {
	//1、正数的左移
	int x = 0b11;
	x = (x << 1);//x变成0b110;
	cout << x << endl;//相当于乘2
	// x << y 相当于x * 2的y次幂

	//2、负数的左移
	int y = -1;
	y = (y << 1);
	cout << y << endl;//结果为-2

	//3、左移负数位
	int x = 64;
	//z = (z << (-1))//不能这样写会报错

	//4、左移溢出
	int a = 64;//二进制为0b1000000
	a = (a << 31);//结果为0，因为1被左移31位超出界外
	return 0;
}