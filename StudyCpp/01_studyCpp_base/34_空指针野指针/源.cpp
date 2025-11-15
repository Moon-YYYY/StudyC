#include<iostream>
using namespace std;
//空指针野指针
int main() {
	//1.空指针
	int* p = NULL;
	cout << *p << endl; //运行时错误，空指针解引用
	//2.野指针
	int* q = (int*)0x121412;//乱地址,野指针
	cout << *q << endl; //运行时错误，野指针解引用
	return 0;
}