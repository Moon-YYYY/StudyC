#include<iostream>
#include<vector>
//vector对象创建
using namespace std;
void printVector(vector<int>& v) {
	for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
		cout << *iter << ' ';
	}
	cout << endl;
}
int main() {
	//1、默认构造函数
	vector<int> v1;
	//2、初始化列表
	vector<int> v2 = { 0, 1, 2, 3, 4 };//隐式构造
	vector<int> v2_2({ 0, 2, 3, 4, 5, 6, 67 });//显式构造

	//3、迭代器
	vector<int>v3(v2.begin(), v2.end());
	printVector(v3);
	//4、全零初始化
	vector<int> v4(8);
	printVector(v4);
	//5、vector<int>变量名(a, b); 申请a个空间的元素，每个元素初始化为b
	vector<int>v5(5, 3);
	printVector(v5);
	//6、拷贝构造函数
	vector<int>v6(v2);
	printVector(v6);
	v6[1] = 6;//并不会改变v2的值
	printVector(v6);
	printVector(v2);
	return 0;
}
