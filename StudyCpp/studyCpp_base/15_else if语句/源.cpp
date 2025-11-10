#include<iostream>
using namespace std;
//else-if语句
int main() {
	int a = 9;
	if (a == 0) {
		cout << "a等于0" << endl;
	}
	else if (a == 3) {
		cout << "a等于3" << endl;
	}
	else if (a == 9) {
		cout << "a等于9" << endl;
	}
	else {
		cout << "a等于10" << endl;
	}
	//如果其他if前面不加else 那么最后的else只会和最近的if做匹配
	//如果除了第一个if之外都是else if那么所有的if或者else if都不满足的时候才会执行最后的else
	//两者的区别在于本节的else if全部不满足才执行else ，对于多个if  ，else只会在最近的if不满足之后才会被执行
	return 0;
}