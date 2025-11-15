#include<iostream>
using namespace std;
/*
this指针
1、解决命名冲突
2、*this就可以获取到这个对象本身

this		*this
&h			*（&h） == h

*/

class Hero1
{
public:
	Hero1(int hp)
	{
		this->hp = hp;
		cout << this << endl;//打印this，实际上是一个指针
		cout << (*this).hp << endl;
		cout << this->hp << endl;
		cout << "-----" << endl;
		cout << &(this->hp) << endl;//分隔符这两个输出的地址相同
		cout << &((*this).hp) << endl;
		cout << "-----" << endl;
		cout << this << endl;//this默认指向hp的地址，因为hp写在了前面，默认指向当前对象的起始地址，hp是第一个声明的非静态数据成员。
		cout << &hp2 << endl;
		cout << "-----" << endl;
	}
	int hp = 1;
	int hp2 = 3;
};
class Hero2
{
	Hero2(int hp1)
	{
		hp1 = hp1;//荧光笔的变量是同一个，所以此赋值无意义，Hero1使用this指针
	}
	int hp1 = 2;
};
int main()
{
	Hero1 h(100);
	cout << h.hp << endl;
	cout << (&h) << endl;//和this的地址相同
	cout << (*(&h)).hp << endl;
	return 0;
}
