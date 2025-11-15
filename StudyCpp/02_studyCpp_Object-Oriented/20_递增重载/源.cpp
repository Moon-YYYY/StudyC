#include<iostream>

using namespace std;
class complex {
	friend ostream& operator<<(ostream& cout, const complex& a); //const complex& a可以不调用拷贝构造函数，节省资源
public:
	complex() :real(0), image(0) {

	}
	complex(int real, int image) {
		this->real = real;
		this->image = image;
	}
	/*complex& operator++() {//加引用是为了返回自己，不加的话++(++a)只会被执行一次，此为前置++
		this->real += 1;
		return *this;
	}*/
	complex operator++(int) {//函数名字后边不加&是因为c为临时对象，不能返回临时对象本身因为函数结束之后c被销毁，成为悬空引用出现未定义行为，应该返回值
		complex c = *this;
		this->real += 1;
		return c;//返回值是递增之前的值，因为++操作后置的时候，等于号会先读取值再自增，所以自增完成之后，返回递增之前的值，这样就实现了递增运算的++后置先取值的效果
	}
private:
	int real;
	int image;
};

ostream& operator<<(ostream& cout, const complex& a) {
	cout << a.real << "+" << a.image << 'i';
	return cout;
}
using namespace std;
int main()
{
	complex a(10, 10);
	cout << a << endl;
	/*++a;
	cout << ++(++a) << endl;//加了两次，如果重载函数不加引用的话则只会加一次，因为(++a)生成了一个新的对象，而不是a本身
	*/
	complex b = a++;
	cout << b << endl;
	cout << a++ << endl;
	cout << a << endl;
}
