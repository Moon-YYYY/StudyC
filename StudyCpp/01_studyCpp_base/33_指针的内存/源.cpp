#include <iostream>
using namespace std;
//指针的内存:所有指针变量占用8字节的内存（64位系统下），如果是32位系统，则占用4字节的内存。
int main() {
	int a = 10;
	int* p = &a; 
	cout << "指针变量p占用的内存大小为： " << sizeof(p) << "  *p也就是a字节大小为： " << sizeof(*p) << endl;//输出8 4
	return 0;
}