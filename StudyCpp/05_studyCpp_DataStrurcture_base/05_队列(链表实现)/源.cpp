#include<iostream>
#include<stdexcept>
using namespace std;
//队列（链表实现）
template<typename T>
class Queue {
private:
	struct Node {
		T data;
		Node* next;
		Node(T value) :data(value), next(NULL) {}
	};
	int size;
	//在此处设置的是Node* 类型的front和rear是因为
	//1、Node类型无法表示空队列
	//2、操作时还涉及到不必要的节点复制
	//3、内存管理复杂
	Node* front;
	Node* rear;
public:
	Queue() :size(0), front(NULL), rear(NULL) {}
	~Queue();
	void enqueue(T element);
	T dequeue();
	T getFront()const;
	int getSize()const;
};
template<typename T>
Queue<T>::~Queue() {
	while (front != NULL) {
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
		Node* tmp = new Node(element);
		rear->next = tmp;
		rear = rear->next;
	}
	++size;
}
template<typename T>
T Queue<T>::dequeue() {
	if (front == NULL) {
		throw underflow_error("Queue is empty");
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
T Queue<T>::getFront()const {
	if (size == 0) {
		throw underflow_error("queue is empty");
	}
	return front->data;
}
template<typename T>
int Queue<T>::getSize()const {
	return size;
}
int main() {
	Queue<int>que;
	que.enqueue(10);
	cout << que.getFront() << endl;
	que.enqueue(1314);
	cout << que.getFront() << endl;
	que.dequeue();
	cout << que.getFront() << endl;
	cout << que.getSize() << endl;

	return 0;
}