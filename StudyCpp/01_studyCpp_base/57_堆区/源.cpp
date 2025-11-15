#include<iostream>
using namespace std;
//堆区
int main() {
	//1、动态分配的单个对象
	int* p = new int(15);//分配在堆区，值为15
	//2、动态分配的数组
	int* q = new int[10](0);//分配在堆区，数组大小为10，每个元素初始化为0
	//3、其他动态内存块

	delete p;
	delete[] q;//delete方式不同
	return 0;
}