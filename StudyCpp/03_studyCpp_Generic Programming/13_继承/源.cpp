#include<iostream>
using namespace std;
//ผฬณะ
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
template<class T1, class T2, class T3>
class Herson :public Hero <T1, T2> {
	T3 a;
};
int main() {

	return 0;
}
