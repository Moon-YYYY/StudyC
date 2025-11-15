#include<iostream>
using namespace std;
//数组申请释放
int* getline(int* Arr, int size) {
	int* ptr = new int[size - 1];
	for (int i = 0; i < size - 1; ++i) {
		ptr[i] = Arr[1 + i] - Arr[i];
	}
	return ptr;
}
int main() {
	int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int* p = getline(arr, 10);
	for (int i = 0; i < 9; ++i) {
		cout << p[i] << " "; //输出1,1,1,1,1,1,1,1一堆1
	}
	cout << endl;
	delete[] p;
	p = NULL;
	return 0;
}