#include <iostream>
using namespace std;
//函数的调用
int max(int a, int b) {//a和b是形参
	return a > b ? a : b;
}
int main() {
	int c = max(max(2, 3), 5);//支持嵌套调用
	int d = max(c, 7);//此处传入c和7，是实参，
	cout << d << endl;
	return 0;
}