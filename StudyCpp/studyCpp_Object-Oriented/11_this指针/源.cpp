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
	}
	int hp;
};
class Hero2
{
	Hero2(int hp1)
	{
		hp1 = hp1;//荧光笔的变量是同一个，所以此赋值无意义，Hero1使用this指针
	}
	int hp1;
};
int main()
{
	Hero1 h(100);
	cout << h.hp << endl;
	cout << (&h) << endl;//和this的地址相同
	cout << (*(&h)).hp << endl;
	return 0;
}
