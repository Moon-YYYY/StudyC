#include <iostream>
using namespace std;
//指针传参，函数值的传递
void swap(int a, int b) {
	int tmp = a;
	a = b;
	b = tmp;
}
void swap2(int* a, int* b) {//指针变量作为参数，可以直接修改指针指向的地址的值
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
int main() {
	int x = 10, y = 20;
	cout << "调用 swap之前: x = " << x << ", y = " << y << endl;
	swap(x, y);
	cout << "调用 swap之后: x = " << x << ", y = " << y << endl;//x和y的值没有被交换，因为只是拷贝了一份数据到swap函数
	cout << "调用 swap2之前: x = " << x << ", y = " << y << endl;
	swap2(&x, &y);
	cout << "调用 swap2之后: x = " << x << ", y = " << y << endl;//x和y的值已经被交换了，因为swap2的形参是指针变量，并且已经分别指向了x和y的地址。
	return 0;
}