#include<iostream>
using namespace std;
//类模板：动态数组类
class DynamicArray {//动态数组类实现完成
private:
	int* elements;
	int size;
public:
	DynamicArray(int n) : size(n) {
		elements = new int[n];
	}
	~DynamicArray() {
		delete[]elements;
	}
	int& operator[](int index) {//为了实现随机访问
		return elements[index];
	}
};

int main() {
	int n = 10;
	//int a[n] = { 0 };.//此方法不可行，要求数组必须是有限长度
	int* p = new int[n];//在堆申请了n * sizeof的字节，动态数组
	DynamicArray da(100);
	da[1] = 3;
	da[9] = 4;
	cout << da[0] << ' ' << da[1] << endl;

	return 0;
}
