#include<iostream>
using namespace std;
//类模板：成员函数（类内定义）
template<typename T>/* typename也可以写成class 但是和class类不一样*/
class DynamicArray {//动态数组类实现完成
private:
	T* elements;
	int size;
public:
	DynamicArray(int n) : size(n) {
		elements = new T[n];

	}
	~DynamicArray() {
		delete[]elements;
	}
	T& operator[](int index) {
		cout << " 调用operator[]函数" << endl;
		return elements[index];
	}
	//更新第index个索引的元素，把他的值改成value
	void update(int index, T value) {//value前面加T类型，是防止在主函数传入参数的时候产生错误，比如传入的是u，但是value是int类型就不是自己想要的结果了
		elements[index] = value;
		cout << "调用updata函数" << endl;
	}
};

int main() {
	DynamicArray<char> dac(100);
	dac[56] = 97;
	cout << dac[56] << endl;//输出a，因为ASCII码97对应的字符是a
	dac[56] = 'h';
	cout << dac[56] << endl;//输出h
	dac.update(56, 'u');
	cout << dac[56] << endl;
	return 0;
}
