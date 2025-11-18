#include<iostream>
#include<string>
//string对象创建
using namespace std;
int main() {
	string s1;//1、无参构造
	cout << s1 << endl;//什么都打印不出来

	string s2({ 'h' , 'e', 'l' , 'l', 'o' });//2、初始化列表
	cout << s2 << endl;

	string s3("英雄哪里出来");//3、字符串初始化
	cout << s3 << endl;

	string s4("英雄哪里出来", 5);//4、字符串的前n个字符,这里的数字表示字节，这里一个汉字占了两个字节
	cout << s4 << endl;//如果字节数是奇数5那只能输出两个汉字

	string s5(s4);//5、拷贝构造函数
	cout << s5 << endl;

	string s6(8, 'o');//6、a个b字符
	cout << s6 << endl;//输出8个o

	return 0;
}