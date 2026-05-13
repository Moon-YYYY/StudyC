#include<iostream>
#include<cstring>
using namespace std;
//高精度框架
const int Base = 1000;
const int capacity = 100;
class BigInt {
private:
	int m_data[capacity];
	int m_size;
public:
	BigInt();//构造函数
	BigInt(const BigInt& bi);//拷贝构造
	BigInt(char s[]);//将字符串中的数字解析成int类型并且存储到data数组里
	BigInt& operator=(const BigInt& bi);//=重载
	BigInt operator+(const BigInt& bi);//加法重载
	void print(char end);//打印
};
BigInt::BigInt() :m_size(0) {
	memset(m_data, 0, sizeof(m_data));//将m_data初始化为0
}
BigInt::BigInt(const BigInt& bi) :m_size(bi.m_size){
	memcpy(m_data, bi.m_data, sizeof(bi.m_data));//从bi拷贝到this
}
BigInt::BigInt(char s[]) {
	int b = 1;//记录当前数位对应的权值，10，100，1000，10000...
	m_size = 0;
	m_data[m_size] = 0;
	for (int i = strlen(s) - 1; i >= 0; --i) {
		m_data[m_size] += (s[i] - '0')* b; //将字符串s里面的字符类型的数字减字符‘0’，转化成数字int类型
		b *= 10;//b每次×10
		if (b >= Base) {
			b = 1;
			m_size++;
			m_data[m_size] = 0;
		}
	}
	if (m_data[m_size] > 0) {//如果最高位有数据，让size加1，最高位应该没有数据
		m_size++;
	}
}
BigInt& BigInt::operator=(const BigInt& bi) {
	m_size = bi.m_size;
	memcpy(m_data, bi.m_data, sizeof(bi.m_data));//重载=
	return *this;
}
BigInt BigInt::operator+(const BigInt& bi) {
	BigInt ret;
	int i = 0, carry = 0;
	for (i = 0; i < m_size || i < bi.m_size || carry > 0; ++i) {
		if (i < m_size) { carry += m_data[i]; }
		if (i < bi.m_size) { carry += bi.m_data[i]; }
		ret.m_data[i] = carry % Base;
		carry /= Base;
	}
	ret.m_size = i;
	return ret;
}
void BigInt::print(char end) {//传入最后想打印的数据
	cout << (m_size == 0 ? 0 : m_data[m_size - 1]);
	for (int i = m_size - 2; i >= 0; --i) {//从高位到低位打印
		for (int j = Base / 10; j > 0; j /= 10) {//j = 100 10 1
			cout << (m_data[i] / j) % 10;
		}
	}
	cout << end;
}
int main() {
	char s[1000];
	char s2[1000];
		while (cin >> s >> s2) {
			BigInt b(s);
			BigInt b2(s2);
			b2 = b2 + b;
			b2.print('\n');
	}
	return 0;
}