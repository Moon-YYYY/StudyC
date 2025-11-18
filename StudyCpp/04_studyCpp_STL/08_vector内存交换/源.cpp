#include<iostream>
#include<vector>
//vector内存交换
using namespace std;
void printVector(vector<int>& v) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		cout << *it << ' ';
	}
	cout << endl;
}
int main() {
	//内存交换
	vector<int> v1 = { 9, 8, 7, 6, 5 };
	vector<int> v2 = { 1, 2, 3, 4, 5, 6 };
	cout << "v1" << endl;
	printVector(v1);
	cout << "v2" << endl;
	printVector(v2);
	v1.swap(v2);//内存交换
	cout << "v1" << endl;
	printVector(v1);
	cout << "v2" << endl;
	printVector(v2);
	//缩容
	v1.resize(100000);
	v1.resize(5);
	cout << "v1.capacity:" << v1.capacity() << endl;//缩容之后 capacity并不会变回去
	/*111*/vector<int>(v1).swap(v1);//生成一个新的对象但是没写变量名 和v1进行内存交换 这个新的对象在生成的时候会变得和v1的元素一样但是内存没有被扩容
	cout << "x.capacity:" << v1.capacity() << endl;
	//拆解上边的语句，开始
	vector<int> v3 = { 1, 2, 3, 4, 5, 6, 7, 8, };
	v3.resize(100000);//v3的capacity会变成100000
	vector<int>x(v3);//构造函数，x变成v3
	x.swap(v3);//和未扩容的x交换
	cout << x.capacity() << endl;
	//为止，这段语句就是拆分注释111语句的步骤
	//3、内存清理
	v2.resize(10000);
	v2.clear();
	cout << v2.capacity() << endl;//即使clear掉了v2的capacity仍然是10000
	vector<int>().swap(v2);//空的初始化列表和v2交换这样可以清理掉v2的内存
	cout << v2.capacity() << endl;
	return 0;
}