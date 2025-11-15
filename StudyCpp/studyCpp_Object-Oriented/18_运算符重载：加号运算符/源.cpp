#include<iostream>
#include<string>
using namespace std;
/*
运算符重载，加号重载
+
//复数类

*/
class complex
{
	friend complex operator+(complex& a, complex& b);
public:
	complex() :real(0), image(0) {

	}
	complex(int real, int image) {// 主函数执行complex a(10, 20);complex b(5, 8);时会调用此函数

		this->real = real;
		this->image = image;
		cout << '-' << endl;
	}
	int GetReal(){
		return image;
	}
	void printf(){
		cout << real << ' ' << image << endl;
	}
private:
	int real;
	int image;
};
complex operator+(complex& d, complex& e)//主函数调用complex c = a + b;时候会调用此函数
{
	complex ret;
	ret.real = d.real + e.real;
	ret.image = d.image + e.image;
	cout << d.real << e.image << endl;
	return ret;
}
int main()
{
	complex a(10, 20);
	complex b(5, 8);
	//cout << b.GetReal() << endl;//获取a或者b的real和image，a和b的real与image是不同的
	complex c = a + b;

	c.printf();//也可以获取a或者b的real和image
	/*
	* 把类看成不同的人进到一个屋子（类）里面，他就会获得不同的铠甲（值或属性），每个人的铠甲是互不干扰的
	*/
	return 0;
}
