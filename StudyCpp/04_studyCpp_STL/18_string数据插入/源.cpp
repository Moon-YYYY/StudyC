#include<iostream>
#include<string>
//string 数据插入
using namespace std;
int main() {
	string s1 = "heoworld";
	s1.insert(2, 2, '1');//表示 在第二个位置插入两个l，第一个2表示位置，第二个2代表插入元素的数量
	cout << s1 << endl;
	s1.insert(4, "0000");//在第四个位置插入字符串
	cout << s1 << endl;
	s1.insert(s1.size(), "hhhh");//获取容器长度插入到字符串尾部
	cout << s1 << endl;
	s1.insert(s1.begin(), '1');//在起始位置插入1
	cout << s1 << endl;
	return 0;
}