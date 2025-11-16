#include<iostream>
using namespace std;
//类模板定义，解释了返回值为引用的原因
template<typename T>/* typename也可以写成class 但是和class类不一样*/
class DynamicArray {//动态数组类实现完成
private:
	T* elements;
	int size;
public:
	DynamicArray(int n) : size(n) {
		elements = new T[size];
	}
	~DynamicArray() {
		delete[]elements;
	}
	T& operator[](int index) {//此处加引用返回值，编译器会将数值赋值给elements[index],而不是修改某个临时拷贝
		return elements[index];
	}
};

int main() {
	int n = 10;
	//int a[n] = { 0 };.//此方法不可行，要求数组必须是有限长度
	int* p = new int[n];//在堆申请了n*sizeof的字节，动态数组
	DynamicArray<double> da(100);//类模板类名<数据类型> 变量名（）;
	da[1] = 3;//调用函数重载，返回值是引用，可以修改da对象中的数据
	da[9] = 4;
	cout << da[0] << ' ' << da[1] << endl;

	DynamicArray<char>dac(10);
	dac[1] = 'A';
	dac[2] = 'C';
	dac[3] = 'M';
	cout << dac[1] << dac[2] << dac[3] << endl;
	return 0;
}
