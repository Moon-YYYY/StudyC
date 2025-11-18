#include<iostream>
#include<deque>
//deque赋值
using namespace std;
void printDeque(deque<int>& d) {
	for (deque<int>::iterator it = d.begin(); it != d.end(); ++it) {
		cout << *it << ' ';
	}
	cout << endl;
}
int main() {
	deque<int> d = { 9, 8, 5, 2, 1, 1 };
	printDeque(d);

	//1、=赋值
	deque<int>d1;
	d1 = d;//赋值，如果在上一行就用=，走的是构造函数，在这一行用=走的是赋值逻辑

	//2、assign（迭代器）
	deque<int>d2;
	d2.assign(d1.begin() + 1, d1.end());// +1,则掠过第一位
	cout << "d2 : ";
	printDeque(d2);

	//3、初始化列表
	deque<int>d3;
	d3.assign({ 1, 3, 4, 6, 8, 4 });
	cout << "d3 : ";
	printDeque(d3);
	
	//4、初始化a个b
	deque<int>d4;
	d4.assign(8, 6);
	cout << "d4 : ";
	printDeque(d4);

	return 0;
}