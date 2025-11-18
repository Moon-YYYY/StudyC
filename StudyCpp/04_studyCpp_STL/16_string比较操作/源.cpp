#include<iostream>
#include<string>
//string 比较操作
using namespace std;
int main() {
	string s1 = "aab";
	string t11 = "aab";
	int r11 = s1.compare(t11);
	cout << s1 << "compare" << t11 << "->" << r11 << endl;

	string t12 = "aaa";
	int r12 = s1.compare(t12);//"aab"和"aaa"比  aab是大的所以返回1
	cout << s1 << "compare" << t12 << "->" << r12 << endl;

	string t13 = "aac";
	int r13 = s1.compare(t13); //"aab" 和"aac"比  aab是小的 所以返回-1
	cout << s1 << "compare" << t13 << "->" << r13 << endl;

	string t14 = "aaba";
	int r14 = s1.compare(t14);//aab 比 aaba短 所以无论第四位是什么都返回-1
	//但是如果和短的逐位比较时，短的字母更大那么短的返回1，比如aac.compare(aaba)返回1
	cout << s1 << "compare" << t14 << "->" << r14 << endl;

	string t15 = "aa";//aab比aa长 返回1
	int r15 = s1.compare(t15);
	cout << s1 << "compare" << t15 << "->" << r15 << endl;

	//  <  >  == <=  >= //运算符重载
	cout << s1 << " == " << t11 << " -> " << (s1 == t11) << endl;//相等返回1
	cout << s1 << " == " << t11 << " -> " << (s1 != t11) << endl;//相等判断错误返回0
	cout << s1 << " < " << t12 << " -> " << (s1 < t12) << endl;//不成立返回0


	return 0;
}