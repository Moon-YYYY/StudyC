#include<iostream>
#include<vector>
using namespace std;
void printVector(vector<int>& v) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << ' ';
	}
	cout << endl;
}
//容量：capacity()
//大小：size()
int main() {
	vector<int> v1 = { 2, 33, 5, 6 };
	printVector(v1);
	cout << "v1.size : " << v1.size() << endl;
	cout << "v1.capacity : " << v1.capacity() << endl;
	v1.push_back(5);
	cout << "v1.size : " << v1.size() << endl;
	cout << "v1.capacity : " << v1.capacity() << endl;//capacity会变成原来的1.5倍(扩容系数),如果9 * 1.5 = 13.5 那么会变成13
	v1.resize(18);//变成18个元素，新创建的元素为0
	cout << "v1.size : " << v1.size() << endl;
	cout << "v1.capacity : " << v1.capacity() << endl;//capacity会变成原来的1.5倍(扩容系数),如果9 * 1.5 = 13.5 那么会变成13
	printVector(v1);
	v1.resize(20, 6);//如果想把新创建的元素变成某个数就可以调用此函数
	cout << "v1.size : " << v1.size() << endl;
	cout << "v1.capacity : " << v1.capacity() << endl;//capacity会变成原来的1.5倍(扩容系数),如果9 * 1.5 = 13.5 那么会变成13
	printVector(v1);
	v1.resize(2);//缩容，会保留前两个元素
	cout << "v1.size : " << v1.size() << endl;
	cout << "v1.capacity : " << v1.capacity() << endl;//capacity会变成原来的1.5倍(扩容系数),如果9 * 1.5 = 13.5 那么会变成13
	printVector(v1);
	return 0;
}
