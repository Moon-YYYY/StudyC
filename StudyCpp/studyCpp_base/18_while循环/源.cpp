#include<iostream>
using namespace std;
//while循环：重复执行某一段代码
int main() {
	int count = 100;
	while (count) {//将count数值传入while循环，并且判断数值是0还是非0，如果是0则跳出循环否则进行下一次循环
		cout << "hello world! " << endl;
		count--;//每次执行时count都进行减1操作，直到count变成0则会退出循环
	}
	int a, b;
	while (cin >> a >> b) {//每次循环之前先等待用户输入，输入完成后执行里面的语句
		cout << a + b << endl;
	}

	return 0;
}