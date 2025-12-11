#include<iostream>
#include<deque>
//deque数据插入
using namespace std;
void printDeque(deque<int>& d) {
	for (int i = 0; i < d.size(); ++i) {
		cout << d[i] << ' ';
	}
	cout << endl;
}
int main() {
	deque<int>d;
	//1、push_front//头插法
	d.push_front(-1);
	d.push_front(-2);
	d.push_front(-3);
	printDeque(d);

	//2、push_back//尾插法
	d.push_back(1);
	d.push_back(2);
	d.push_back(3);
	printDeque(d);//-3 -2 -1 1 2 3 

	//3、insert
	d.insert(d.begin() + 3, 0);//在下标为3的位置前面插入0
	printDeque(d);
	d.insert(d.end() - 1, 5, 8);//在最后一个元素的前面插入5个8
	printDeque(d);
	d.insert(d.begin() + 1, d.begin() + 4, d.begin() + 6);//在下标为1的前面插入，插入d的下标为4到6的元素，左闭右开，6取不到
	printDeque(d);
	return 0;
}