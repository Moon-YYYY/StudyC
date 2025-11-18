#include<iostream>
#include<string>
//string 子串获取
using namespace std;
int main() {
	string s1;
	s1 = "hello wooooooorld";
	string s2 = s1.substr(7, 4);//从第七个字符取四个
	cout << s2 << endl;

	string s3 = "英雄哪里出来&&夜深人静写算法";
	int pos = s3.find("&&");
	string s4 = s3.substr(0, pos);//从下标为0取到下标为pos
	string s5 = s3.substr(pos + 2);//第二个参数不写则直接获取到末尾
	cout << s4 << ' ' << s5 << endl;

	return 0;
}