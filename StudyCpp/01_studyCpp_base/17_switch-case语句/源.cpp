#include<iostream>
using namespace std;
/*
switch - case语句 switch中传入一个值  循环中判断这个值，满足哪个case就执行哪个语句为了不让其死循环
所以每个case语句最后都加break跳出循环
*/
int main() {
	int a = 0;
	cin >> a;
	switch (a) {
	case 0:
		cout << "zero" << endl;
		break;
	case 1:
		cout << "one" << endl;
		break;
	case 2:
		cout << "two" << endl;
		break;
	default: 
		cout << "no find" << endl;
		break;
	}//输入2，输出two

	return 0;
}