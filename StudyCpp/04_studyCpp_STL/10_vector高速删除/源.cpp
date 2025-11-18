#include<iostream>
#include<vector>
//vector高速删除
using namespace std;
void remove1(vector<int>& v, int index) {
	v.erase(v.begin() + index);//效率低
}
void remove2(vector<int>& v, int index) {
	swap(v[index], v.back());
	v.pop_back();//对顺序要求不高但是对小效率要求高的算法
}
void printVector(const vector<int>& v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i] << ' ';
	}
	cout << endl;
}
int main() {

	vector<int> v = { 1, 2, 3, 4, 5, 5, 5, 5, 5,4, 32, 4, 5, 6 };
	for (int i = 0; i < 1000000; ++i) {
		v.push_back(i);
	}
	for (int i = 0; i < 100000; ++i) {
		remove2(v, 4);
	}
	return 0;
}