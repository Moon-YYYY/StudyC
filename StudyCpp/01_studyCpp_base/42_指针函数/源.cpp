#include <iostream>
using namespace std;
//指针函数
int* getArray(int a, int d, int n) {
	int* ret = new int[n];//在地址上申请一块内存空间
	for (int i = 0; i < n; ++i) {
		ret[i] = a + i * d;
	}
	return ret;
}
int main() {
	int* str = getArray(5, 3, 8);//把外部函数传回来的数组首地址接收一下，通过首地址就可以的读取到地址中的元素
	for (int i = 0; i < 8; ++i) {
		cout << str[i] << " ";//也可以用*(str + i)来访问数组元素
	}
	return 0;
}