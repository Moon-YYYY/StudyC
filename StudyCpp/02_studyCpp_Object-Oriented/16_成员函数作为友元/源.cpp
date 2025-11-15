#include<iostream>
#include<string>
using namespace std;
//成员函数作为友元
//PeopleFriend 的某个函数能访问People的私有成员函数
class People;
class PeopleFriend{
public:
	PeopleFriend() {}
	void visitALL(People* p);
	void visitPub(People* p);
};

class People {
	friend class PeopleFriend;/*此写法其他类的所有函数都可以访问本类的私有成员*/
	friend void PeopleFriend::visitALL(People* p);/*此写法可以指定其他类的某个成员函数可以访问此类的私有成员*/
public:
	People(){
		m_House = "别墅";
		m_Car = "跑车";
	}
public:
	string m_House;
private:
	string m_Car;
};
void PeopleFriend::visitALL(People* p)
{
	cout << "好朋友访问了你的" << p->m_House << endl;
	cout << "好朋友访问了你的" << p->m_Car << endl;
}

void PeopleFriend::visitPub(People* p)
{
	cout << "好朋友访问了你的" << p->m_House << endl;
	cout << "好朋友访问了你的" << p->m_Car << endl;

}
int main()
{
	People p;
	PeopleFriend pf;
	pf.visitALL(&p);
	pf.visitPub(&p);
	return 0;
}
