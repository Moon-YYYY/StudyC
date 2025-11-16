#include<iostream>
using namespace std;
//多参数函数模板以及打印参数类型
/*
函数模板
template<typename T>
函数的定义
*/
template<typename T1, typename T2, typename T3>
T1 add(T2 a, T3 b)
{
	T1 c = a + b;
	return c;
}
template<typename A2, typename A3>//函数模板支持多个类型参数，一旦有类型不能推导就会导致编译失败
A2 Add(A2 a, A3 b) {
	cout << typeid(A2).name() << endl;//可以打印参数类型
	return A2;//返回值类型无法作为推导依据，报错
}
int main()
{

	//double r = add(4.0, 8);//返回值类型无法作为推导依据，T1推导不出来，把T1改成T2就可以了，因为T2在传入参数的时候就已经指定了T2是double
	cout << Add(2, 3) << endl;
	return 0;
}
