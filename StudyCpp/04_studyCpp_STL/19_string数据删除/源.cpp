#include<iostream>
#include<string>
//string数据删除
using namespace std;
int main() {
	string s1 = "hellooooworld";

	s1.erase();//全部删除
	cout << s1 << endl;

	s1 = "hellooooworld";
	s1.erase(7);//第七个字符后面的所有的字符全部删除
	cout << s1 << endl;

	s1.erase(4, 3);//从第四个位置删除三个
	cout << s1 << endl;

	s1.erase(s1.begin());//删除第一个元素
	cout << s1 << endl;
	s1 = "1234567891011";
	s1.erase(s1.begin() + 5, s1.begin() + 10);//从下标第五个删除到下标第十个
	cout << s1 << endl;

	return 0;
}