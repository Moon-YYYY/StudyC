#include<iostream>
#include<string>
//string数据查找
using namespace std;
int main() {
	string s1 = "hello wooooooorld";
	cout << s1.find("oooo") << endl;//输出7，表示从下标第七位的位置找到了这个字符串

	cout << (int)s1.find("oooo", 8) << endl;//表示从第八个位置开始找，返回值转化为int类型，如果是-1则是
	//找不到，如果找到了则返回下标
	cout << s1.find('o') << endl;//返回4，第四位找到了o

	cout << s1.find('o', s1.find('o') + 1) << endl;//嵌套调用，来寻找第二个o，后一个find返回找到的下标然后加1表示从下一个位置开始寻找

	cout << s1.rfind("oo") << endl;//返回11，从右往左找，找到的话返回正顺序下标的值

	return 0;
}