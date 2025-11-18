#include<iostream>
#include<vector>
using namespace std;
//vector随机访问
void printVector(vector<int>& v) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		cout << *it << endl;
	}
	cout << endl;
}
int main() {
	vector<int> v = { 9, 8, 7, 6, 5 };
	cout << v[1] << endl;//和数组的下标访问是一样的，如果确保不会下标越界，用中括号版本的效率会更高一些
	cout << v.at(1) << endl;//无论是debug版本还是release版本都会有下标越界保护，越界会产生异常
	cout << v.front() + 1 << endl;//输出为10，第一个元素加一然后输出，但是第一个元素的值并未改变
	cout << v.front() << endl;//访问第一个元素，值还是9
	cout << *(v.begin() + 1)  << endl;//访问第二个元素
	return 0;
}