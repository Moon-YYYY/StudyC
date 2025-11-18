#include<iostream>
#include<vector>
//vector 基础概念
using namespace std;
int main() {
	int a[6] = { 9, 8, 7, 6, 5, 4 };
	vector<int> v = { 2, 0, 2, 4 };
	cout << v.capacity() << endl;//打印的是总容量，而不是占用容量
	v.push_back(7);//此语句作用是在尾部加一个7
	cout << v.capacity() << endl;
	/*
	front()代表vector的第一个元素的值，把back()代表着最后一个元素的值
	begin()代表指向第一个元素，end()代表指向最后一个元素的下一个元素，叫做左闭右开，左边能取到值，右边不能
	*/
	cout << "begin->" << *v.begin() << endl; //迭代器是begin和end，对迭代器加上*表示打开了begin的房门取里面的值
	cout << "end->" << *(v.end() - 1) << endl;//打开end的房门会出错因为end指向的是最后一个元素的下一个元素，这个地方是没有东西的所以需要减一
	cout << "front:" << v.front() << endl;//front是vector的第一个元素的值
	cout << "back:" << v.back() << endl;//back是vector的最后一个元素的值

	return 0;
}
