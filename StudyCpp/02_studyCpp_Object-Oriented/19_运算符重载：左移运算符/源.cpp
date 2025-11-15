#include<iostream>
using namespace std;
/*
左移重载 <<
cout.operator <<(cout , c)
//友元函数左移重载
c.operator<<(cout);
*/

class complex
{
	friend ostream& operator<<(ostream& out, complex a);
	friend complex operator+(complex& a, complex& b);
public:
	complex() :real(0), image(0) {}
	complex(int real, int image){
		this->real = real;
		this->image = image;
	}
private:
	int real;
	int image;
};
complex operator+(complex& a, complex& b){
	complex ret;
	ret.real = a.real + b.real;
	ret.image = a.image + b.image;
	return ret;
}
ostream& operator<<(ostream&out ,complex a){//加引用是为了防止调用拷贝构造函数，而拷贝构造函数已经被delete cout, complex a) {//左移运算符重载，函数类型加引用是为了实现链式操作
	out << a.real << '+' << a.image << 'i';
	return out;
}
int main()
{
	complex a(3, 8);
	complex b(5, 9);
	complex c = a + b;
	operator<<(cout, c);//可以运行，因为重载函数中的两个参数和传入的两个参数类型一致，所以可以调用，但是如果写成operator<<(cout, endl);就不行了因为重载函数里面的形参类型识别不了endl
	cout << c << endl;//链式操作是指cout << a << '+' << b << 'i';多个<<
	//此处先传入到重载函数中两个参数一个是cout，一个是c，然后返回值是cout，然后进行链式操作，cout << endl;
	return 0;
}
