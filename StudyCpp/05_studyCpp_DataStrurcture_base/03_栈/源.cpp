#include<iostream>
#include<stdexcept>
using namespace std;
//ջ
template<typename T>
class stack {
private:
	int size;
	T* data;
	int total;
	void resize();
public:
	stack() : data(new T[10]), size(0), total(10) {}
	~stack();
	void push(T value);
	T pop();
	T top() const;
	int getsize()const;
};
template<typename T>
void stack<T>::resize() {
	int newTotal = total * 2;
	T* newdata = new T[newTotal];
	for (int i = 0; i < size; ++i) {
		newdata[i] = data[i];
	}
	delete[] data;
	data = newdata;
	total = newTotal;
}
template<typename T>
stack<T>::~stack() {
	delete[] data;
}
template<typename T>
void stack<T>::push(T value) {
	if (size == total) {
		resize();
	}
	data[size++] = value;
}
template<typename T>
T stack<T>::pop() {
	if (size == 0) {
		throw std::underflow_error("stack is empty");
	}
	return data[--size];
}
template<typename T>
T stack<T>::top() const{
	if (size == 0) {
		throw underflow_error("stack is empty");
	}
	return data[size - 1];
}
template<typename T>
int stack<T>::getsize() const{
	return size;
}
int main() {
	stack<int>stk;
	stk.push(10);
	stk.push(13);
	
	cout << stk.top() << endl;
	cout << stk.top() << endl;
	cout << stk.getsize() << endl;
	stk.push(13); stk.push(13); stk.push(13); stk.push(13); stk.push(13); stk.push(13); stk.push(13); stk.push(13);
	cout << stk.top() << endl;
	cout << stk.getsize() << endl;
	stk.push(2);
	cout << stk.getsize() << endl;
	return 0;
}