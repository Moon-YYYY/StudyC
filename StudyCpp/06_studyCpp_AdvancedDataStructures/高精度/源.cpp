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
	BigInt(int v);
	BigInt& operator=(const BigInt& bi);//=重载
	BigInt operator+(const BigInt& bi)const;//加法重载
	BigInt operator-(const BigInt& bi)const;//减法重载
	BigInt operator*(const BigInt& bi)const;//乘法重载
	BigInt operator/(const BigInt& bi)const;//除法重载
	int compare(const BigInt& bi);
	void print(char end);//打印
};
BigInt::BigInt() :m_size(0) {
	memset(m_data, 0, sizeof(m_data));//将m_data初始化为0
}
BigInt::BigInt(const BigInt& bi) :m_size(bi.m_size){
	memcpy(m_data, bi.m_data, sizeof(bi.m_data));//从bi拷贝到this
}
BigInt::BigInt(int v):m_size(0) {
	while (v > 0) {
		m_data[m_size++] = v % Base;
		v /= Base;
	}
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
BigInt BigInt::operator+(const BigInt& bi) const {
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

BigInt BigInt::operator-(const BigInt& bi) const {
	BigInt ret;
	int carry = 0;
	ret.m_size = m_size;
	for (int i = 0; i < ret.m_size; ++i) {
		ret.m_data[i] = m_data[i] - carry;
		if (i < bi.m_size) {
			ret.m_data[i] -= bi.m_data[i];
		}
		if (ret.m_data[i] < 0) {
			carry = 1;
			ret.m_data[i] += Base;
		}
		else {
			carry = 0;
		}
	}
	while (ret.m_size > 0 && ret.m_data[ret.m_size - 1] == 0) {//去除前导零
		--ret.m_size;
	}
	return ret;
}
BigInt BigInt::operator*(const BigInt& bi) const{
	BigInt ret;
	ret.m_size = m_size + bi.m_size;
	for (int i = 0; i < ret.m_size; ++i) {
		ret.m_data[i] = 0;
	}
	for (int i = 0; i < m_size; ++i) {
		int carry = 0;
		for (int j = 0; j < bi.m_size; ++j) {
			ret.m_data[i + j] += m_data[i] * bi.m_data[j] + carry;
			if (ret.m_data[i + j] >= Base) {
				carry = ret.m_data[i + j] / Base;
				ret.m_data[i + j] %= Base;
			}
			else {
				carry = 0;
			}
		}
		ret.m_data[i + bi.m_size] += carry;
	}
	while (ret.m_size > 0 && ret.m_data[ret.m_size - 1] == 0) {//去除前导零
		--ret.m_size;
	}
	return ret;
}
BigInt BigInt::operator/(const BigInt& bi) const {
	BigInt ret;
	BigInt carry = 0;
	int left, right, mid;
	for (int i = m_size - 1; i >= 0; --i) {
		carry = carry * Base + m_data[i];
		left = -1;
		right = Base;
		while (left + 1 < right) {
			mid = (left + right) / 2;
			if ((bi * mid).compare(carry) <= 0) {
				left = mid;
			}
			else {
				right = mid;
			}
		}
		ret.m_data[i] = left;
		carry = carry - bi * left;
	}
	ret.m_size = m_size;
	while (ret.m_size > 0 && ret.m_data[ret.m_size - 1] == 0) {//去除前导零
			--ret.m_size;
	}
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
int BigInt::compare(const BigInt& bi) {
	if (m_size != bi.m_size) {
		return m_size > bi.m_size ? 1 : -1;
	}
	for (int i = m_size - 1; i >= 0; --i) {
		if (m_data[i] != bi.m_data[i]) {
			return m_data[i] > bi.m_data[i] ? 1 : -1;
		}
	}
	return 0;
}
int main() {
	char s[1000];
	char s2[1000];
		while (cin >> s >> s2) {
			BigInt b(s);
			BigInt b2(s2);
			b = b / b2;
			b.print('\n');
	}
	return 0;
}