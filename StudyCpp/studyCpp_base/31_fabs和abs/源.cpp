#include<iostream>
#include<cmath>
using namespace std;
//fabs和abs取绝对值
int main() {
	int x = -10;
	double y = -3.14;
	cout << (abs(x)) << ' ' << (fabs(y)) << endl;//其中abs和fabs都可以取绝对值，但是abs是用于整数，fabs是用于浮点数
	return 0;
}