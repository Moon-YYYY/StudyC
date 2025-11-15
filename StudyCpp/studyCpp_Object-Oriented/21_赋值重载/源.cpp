#include<iostream>

using namespace std;

class Hero {
public:
	Hero() :m_Data(NULL) {}
	Hero(int data) {
		m_Data = new int(data);
	}
	~Hero(){
		if (m_Data) {
			delete m_Data;//double free
			m_Data = NULL;
		}
	}
	Hero& operator=(Hero& h) {//h1 = h2的时候调用此函数
		if (m_Data) {//这里的m_Data是h1的成员变量
			delete m_Data;//释放h1的内存，防止double free
			m_Data = NULL;
		}
		m_Data = new int;
		*m_Data = *h.m_Data;
		return *this;
	}
public:
	int* m_Data;
};
int main()
{
	Hero h1(1);
	Hero h2(2);
	Hero h3(3);

	cout << h1.m_Data << endl;
	cout << h2.m_Data << endl;
	h1 = h2;//不写=重载函数会导致内存泄漏
	cout << h1.m_Data << endl;
	cout << h2.m_Data << endl;

	h3 = (h2 = h1);
	/*
	初始状态：
h1.m_Data → [1] (地址0x1000)
h2.m_Data → [2] (地址0x2000)

执行 h1 = h2 后：
h1.m_Data → [2] (地址0x2000)  // 原0x1000内存泄漏！
h2.m_Data → [2] (地址0x2000)

析构时：
~h2() 释放 0x2000
~h1() 再次尝试释放 0x2000 → double free!
	*/
	return 0;
}
