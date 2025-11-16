#include<iostream>
using namespace std;
//类模板：成员函数（类外定义）
template<typename T>/* 也可以写成class 但是和class类不一样*/
class DynamicArray {//动态数组类实现完成
private:
	T* elements;
	int size;
public:
	DynamicArray(int n);
	~DynamicArray();
	T& operator[](int index);	//更新第index个索引的元素，把他的值改成value
	void update(int index, T value); //value前面加T类型，是防止在主函数传入参数的时候产生错误，比如传入的是u，但是value是int类型就不是自己想要的结果了
};
template<class T>//成员函数类外定义
DynamicArray<T>::DynamicArray(int n) :size(n) {
	elements = new T[n];
}
template<class T>
DynamicArray<T>::~DynamicArray() {
	delete[] elements;
}
template <class T>
T& DynamicArray<T>::operator[](int index) {
	return elements[index];
}
template<class T>
void DynamicArray<T>::update(int index, T value) {
	elements[index] = value;
}
int main() {

	DynamicArray<char> dac(100);
	dac[56] = 'h';
	dac.update(56, 'u');
	cout << dac[56] << endl;
	return 0;
}
