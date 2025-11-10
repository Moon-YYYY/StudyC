#include<iostream>
using namespace std;
//常量指针常量最专一，既不能修改指针指向的地址，也不能修改指针指向的值。
int main() {
	int a = 1;
	int b = 2;
	const int* const p = &a;
	//p = &b;//不可行
	//*p = 3;//不可行
	cout << *p << endl;
	return 0;
}