#include<iostream>
#include<string>
//string数据替换
using namespace std;
int main() {
	string s;
	s = "hello woooorld";
	s.replace(7, 5, "or");//表示从第七位开始选5个元素替换掉，替换成or
	cout << s << endl;

	s = "hello woooorld";
	s.replace(s.begin() + 7, s.begin() + 12, "or");//和上边的操作一样 ，从第七个到第十二个替换成or
	cout << s << endl;

	s = "hello woooorld";
	s.replace(s.begin() + 7, s.begin() + 12, "orerwrwerwer", 2);//表示从参数中的字符串中取前两个作为替换，去替换掉第七个到第十二个
	cout << s << endl;

	return 0;
}