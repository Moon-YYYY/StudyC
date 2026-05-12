#include<iostream>
#include<stdexcept>
using namespace std;
//用队列实现栈
template<typename T>
class Queue {
private:
	struct Node{
		T data;
		Node* next;
		Node(T d) :data(d), next(NULL) {}
	};
	Node* front;
	Node* rear;
	int size;
public:
	Queue() :front(NULL), rear(NULL), size(0) {}
	~Queue();
	void enqueue(T element);
	T dequeue();
	T getFront()const;
	T getSize()const;
};
template<typename T>
Queue<T>::~Queue() {
	while (front) {
		Node* tmp = front;
		front = front->next;
		delete tmp;
	}
}
template<typename T>
void Queue<T>::enqueue(T element) {
	if (rear == NULL) {
		rear = new Node(element);
		front = rear;
	}
	else {
		rear->next = new Node(element);
		rear = rear->next;
	}
	++size;
}
template<typename T>
T Queue<T>::dequeue() {
	if (front == NULL) {
		throw std::underflow_error("Queue is empty");
	}
	T element = front->data;
	Node* tmp = front;
	front = front->next;
	delete tmp;
	--size;
	if (size == 0) {
		rear = NULL;
	}
	return element;
}
template<typename T>
T Queue<T>::getFront() const{
	if (front == NULL) {
		throw underflow_error("Queue is empty");
	}
	return front->data;
}
template<typename T>
T Queue<T>::getSize() const{
	return size;
}
class Mystack {
private:
	Queue<int>q1;
	Queue<int>q2;
public:
	Mystack() {}
	void push(int x) {
		q1.enqueue(x);
	}
	int pop() {
		while (q1.getSize() > 1) {
			q2.enqueue(q1.dequeue());
		}
		int result = q1.dequeue();
		while(q2.getSize() != 0)
		q1.enqueue(q2.dequeue());
		return result;
	}
	int top() {
		while (q1.getSize() > 1) {
			q2.enqueue(q1.dequeue());
		}
		int result = q1.dequeue();
		q2.enqueue(result);
		while (q2.getSize() != 0)
			q1.enqueue(q2.dequeue());
		return result;
	}
	bool empty() {
		return q1.getSize() == 0;
	}
};
int main() {
	
	return 0;
}