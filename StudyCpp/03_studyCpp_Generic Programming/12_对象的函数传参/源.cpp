#include<iostream>
#include<string>
using namespace std;
//对象的函数传参
template<class NameType, class HpType>
class Hero {
public:
	Hero(NameType name, HpType hp) {
		this->m_name = name;
		this->m_hp = hp;
	}
	NameType GetName() {
		return this->m_name;
	}
private:
	NameType m_name;
	HpType m_hp;
};
//1、直接指定类型
void test1(Hero<string, double>& h) {

}
//2、参数模板化
template<typename T1, typename T2>
void test2(Hero<T1, T2>& h) {

}
//3、类模板化
template<typename T>
void test3(T& h) {
	cout << h.GetName() << endl;
}
int main() {
	Hero<string, double>h("剑圣", 100.0);
	test1(h);
	test2(h);
	test3(h);
	return 0;
}
