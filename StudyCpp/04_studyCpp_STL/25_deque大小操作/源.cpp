#include<iostream>
#include<deque>
//deque 大小操作
using namespace std;
void printDeque(deque<int>& d) {
	for (deque<int>::iterator it = d.begin(); it != d.end(); ++it) {
		cout << *it << ' ';
	}
	cout << endl;
}
int main() {
	deque<int>d;
	cout << "d.empty() = " << d.empty() << endl;//是否为空，如果为空则返回1，此处返回1
	cout << "d.size()  = " << d.size() << endl;//返回元素个数
	d.assign({ 1, 2, 3 });
	cout << "d.empty() = " << d.empty() << endl;
	cout << "d.size()  = " << d.size() << endl;
	printDeque(d);
	d.resize(18);//把元素个数变成十八，新增的变成0
	cout << "d.resize() = " << d.size() << endl;
	printDeque(d);
	d.resize(20, 6);//新增的变成6，所以此处最后两个元素是6
	cout << "d.resize() = " << d.size() << endl;
	printDeque(d);
	d.resize(5);//先变小再变回去，数据会丢失
	d.resize(20);
	printDeque(d);
	return 0;
}