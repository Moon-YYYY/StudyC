#include<iostream>
#include<vector>
using namespace std;
void printVector(vector<int>& v) {
	for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
		cout << *iter << ' ';
	}
	cout << endl;
}
//vector数据删除
//pop_back

int main() {
	vector<int> v = { 9, 8, 5, 2, 1, 1 };
	printVector(v);
	v.pop_back();//删除最后一个元素
	printVector(v);
	v.erase(v.begin() + 1);//删除下标为1的元素
	printVector(v);
	vector<int> v2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	printVector(v2);
	vector<int>::iterator it = v2.erase(v2.begin() + 5);
	cout << *it << endl;//输出为删除的元素的下一个元素的值
	printVector(v2);
	v2.clear();//删除全部元素
	printVector(v2);
	return 0;
}
