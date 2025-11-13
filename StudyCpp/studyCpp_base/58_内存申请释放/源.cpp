#include<iostream>
using namespace std;
//内存申请释放
int main() {
	int* ptr = new int(131);
	cout << *ptr << endl;
	int* ptr2 = new int;
	*ptr2 = 121;
	cout << *ptr2 << endl;//两种初始化方式均可
	return 0;
}