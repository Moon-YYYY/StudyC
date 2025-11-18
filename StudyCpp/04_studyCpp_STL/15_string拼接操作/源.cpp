#include<iostream>
#include<string>
//string 拼接操作
using namespace std;

int main() {
	string s1 = "英雄";
	string t1 = "出来";
	s1 += "哪里";//拼接操作
	cout << s1 << endl;
	s1 += t1;//拼接操作， 和+一样
	cout << s1 << endl;

	s1 += ';';//也可以加分号
	cout << s1 << endl; 

	//append
	string s3 = "英雄";
	string t3 = "联盟";
	s3.append("算法");
	s3.append(t3);
	s3.append("12312312", 3);
	cout << s3 << endl;
	s3.append("1234567", 3, 4);
	cout << s3 << endl;

	//push_back
	string s4 = "英雄编程";
	s4.push_back('6');
	s4.push_back('6');
	s4.push_back('6');
	cout << s4 << endl;
	return 0;
}