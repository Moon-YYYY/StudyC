#include<iostream>
#include<bitset>//二进制转换的头文件
using namespace std;
//  | 位或运算符 
int main() {
	//1、位或运算符的定义
	int a = 0b1010;
	int b = 0b0110;
	//a | b  = 1110; 从二进制数的最低位向高位递增 比较 有一个为真则为真
	cout << (a | b) << endl;//输出为14

	//2、设置标记位
	int c = 0b100111;
	//  变成0b101111；
	cout << (c | (0b1000)) << endl;//有一个为真则为真，0b1000和c进行位或 c的第四位变成1 

	//3、置空标记位
	int d = 0b100111;
	int e = 0b000100;
	cout << bitset<10>((e | d) - e) << endl;//变成0b100011，第三位的1被置空

	//4、低位连续的0变成1
	int f = 0b1010010000;
//要想变成  0b1010011111;
  //f - 1 = 0b1010001111;
	cout << bitset<10>(f | (f - 1)) << endl;//bitset是转换为二进制的函数  bitset<转换成几位二进制>(转换目标)
	return 0;
}