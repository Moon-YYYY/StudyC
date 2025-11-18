#include<iostream>
#include<string>
//string 随机访问
using namespace std;
int main() {
	//随机访问
	string s = "hello world";
	cout << s << endl;
	for (int i = 0; i < s.size(); ++i) {//获取字符串长度不包括'\0'
		cout << s[i] << '0';
	}
	for (int i = 0; i < s.size(); ++i) {
		cout << s.at(i) << ' ';//和上面的for循环作用一样，但是at在无论是debug版本哈市release版本有下标越界检测，但是at的效率会低一些
	}
	cout << endl;
	s[11] = '5';//直接修改
	s[5] = '0';
	s.at(2) = '1';
	cout << s << endl; 
	return 0;
}