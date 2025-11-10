#include<iostream>
using namespace std;
/*函数的声明:程序的执行是从上至下找到main函数，然后从main函数开始执行，遇到调用其他函数时，就调用相应的函数。
但是当函数在main之后或者在本函数之后，编译器并不知道该函数的具体位置，所以编译器无法确定调用函数的指令地址。
因此，在调用函数之前，必须先声明函数的原型，即函数的名称、参数类型、返回类型。
这样可以解决函数调用的书写顺序问题。
*/
int add(int a, int b);
void fun2(int);//如果不写函数声明，编译器会报错，因为在main函数调用的时候，fun2函数还没有被编译器识别到。
int main() {
	fun2(10);
	return 0;
}
void fun2(int x) {
	cout << "fun2 : " << x << endl;
	int y = add(3, x);
	cout << "y = " << y << endl;
}
int add(int a, int b) {
	return a + b;
}