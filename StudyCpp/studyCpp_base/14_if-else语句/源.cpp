#include<iostream>
using namespace std;
// if - else语句
int main() {
	//1、if
	int a = 7;
	if (a % 2 != 0) {
		cout << "这是一个奇数" << endl;
	}
	//2、if-else
	/*
	if(表达式){
		表达式1;
	}
	else{
		表达式2;
	}
	*/
	if (a % 2 == 0) {
		cout << "这是一个偶数" << endl;
	}
	else {
		cout << "这是一个奇数" << endl;
	}
	return 0;
}