#include<iostream>
#include<vector>
using namespace std;
//vector数据插入
void printVector(vector<int>& v) {
	for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
		cout << *iter << ' ';
	}
	cout << endl;
}
//vector数据插入
//1、push_back
//2、insert
int main() {
	vector<int> v;
	for (int i = 0; i < 10; ++i) {
		v.push_back(i);//尾部插入元素
	}
	printVector(v);
	v.insert(v.begin(), 888);//插入到第一个元素的前面
	printVector(v);

	v.insert(v.begin() + 5, 666);//让666插入到下标为5的地方
	printVector(v);
	return 0;
}
