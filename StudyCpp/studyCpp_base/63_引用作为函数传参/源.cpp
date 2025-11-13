#include<iostream>
using namespace std;
//引用作为函数传参
int add(int& a, int& b) {
	a++;
	b++;
	return a + b;
}
int main() {
	int x = 1; 
	int y = 1;
	cout << x + y << endl;
	int c = add(x, y);
	cout << x + y << endl;//x和y的值都已经加1了
	cout << c << endl;
	return 0;
}