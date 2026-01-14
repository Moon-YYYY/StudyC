#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstring>

using namespace std;
//串
class String {
private:
	char* str;
	size_t length;
public:
	String();
	String(const char* s);
	~String();
	//拷贝构造函数
	String(const String& s);
	size_t getLength() const;
	char operator[](size_t index) const;
	String& operator=(const String& s);
	bool operator==(const String& s)const;
	bool operator!=(const String& s)const;
	String copy()const;
	String operator+(const String& s)const ;
	friend ostream& operator<<(ostream& out, const String& s);
};
String::String() {
	length = 0;
	str = new char[1];
	str[0] = '\0';
}
String::String(const char* s) {
	length = strlen(s);
	str = new char[length + 1];
	strcpy(str, s);
}
String::~String() {
	delete[] str;
}
size_t String::getLength() const {
	return length;
}
char String::operator[](size_t index) const {
	return str[index];
}
String::String(const String& s) {
	length = s.length;
	str = new char[length + 1];
	strcpy(str, s.str);
}
String& String::operator=(const String& s) {
	//此处采用深拷贝，为了不让str指向的内容改变而导致s.str的内容也发生改变
	//因为此处是赋值操作，只是单纯的赋值，不需要实现str的改变去导致s.str也发生改变
	//所以str在new的时候要写好申请的内存大小，来实现深拷贝
	if (this != &s) {
		delete[]str;
		length = s.length;
		str = new char[length + 1];
		strcpy(str, s.str);
	}
	return *this;
}
bool String::operator==(const String& s) const{
	return strcmp(str, s.str) == 0;
}
bool String::operator!=(const String& s)const {
	return strcmp(str, s.str) != 0;
}
String String::copy()const {
	String s = *this;
	return s;
}
String String::operator+(const String& s)const {
	String result;
	result.length = length + s.length;
	result.str = new char[result.length + 1];
	strcpy(result.str, str);
	strcat(result.str, s.str);
	return result;
}
ostream& operator<<(ostream& out, const String& s) {
	out << s.str;
	return out;
}
int main() {
	String s{ "12345d" };
	cout << s << endl;
	cout << s + "5566" << endl;
	cout << s[3] << endl;
	cout << (s == "12345d") << endl;
	cout << (s != "12345d") << endl;
	s = s + "1234";
	String a, b, c;
	a = b = c = s;
	cout << s << endl;
	cout << a << b << c << endl;
	String x = s.copy();
	cout << x << endl;
	return 0;
}