#include <iostream>
using namespace std;
//指针常量,表示指针的房间里只能放和a房间关联的物品，所以p = &b是错误的
int main() {
	int a = 10;
	int b = 20;
	int* const p = &a;//指针常量,
	//p = &b;//错误,不能修改指针常量的值
	*p = 30;//正确,指针常量可以修改指针指向的变量的值
	cout << a << endl; //输出30
	return 0;
}