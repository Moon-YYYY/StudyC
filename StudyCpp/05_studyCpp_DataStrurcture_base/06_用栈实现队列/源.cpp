#include<iostream>
#include<stdexcept>
using namespace std;
//用栈实现队列
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
	bool isempty()const;
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
T stack<T>::top() const {
	if (size == 0) {
		throw underflow_error("stack is empty");
	}
	return data[size - 1];
}
template<typename T>
int stack<T>::getsize() const {
	return size;
}
template<typename T>
bool stack<T>::isempty()const {
	return size;
}
class MyQueue {
private:
	stack<int>s1;
	stack<int>s2;
public:
	MyQueue() {}
	void push(int x) {
		s1.push(x);
	}
	int pop() {
		if (s2.isempty()) {
			while (!s1.isempty()) {
				s2.push(s1.pop());
			}
		}
		return s2.pop();
	}
	int peek() {
		if (s2.isempty()) {
			while (!s1.isempty()) {
				s2.push(s1.pop());
			}
		}
		return s2.pop();
	}
	bool empty() {
		return s1.isempty() && s2.isempty();
	}
};