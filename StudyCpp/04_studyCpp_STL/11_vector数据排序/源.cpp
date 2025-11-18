#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
void printfVector(const vector<int>& v) {
	for (int i = 0; i < v.size(); ++i) {
		cout << v[i] << ' ';
	}
	cout << endl;
}
bool cmp(int a, int b) {//使用此函数来实现逆序排列
	return a > b;
}
int main() {
	vector<int> v = { 1,2,3,4,5,5,5,4,32,3,4,5,5,4 };
	vector<int> v1 = { 1,2,3,4,5,5,5,4,32,3,4,5,5,4 };
	sort(v.begin(), v.end());//默认递增排序
	sort(v1.begin(), v1.end(), cmp);
	printfVector(v1);
	return 0;
}