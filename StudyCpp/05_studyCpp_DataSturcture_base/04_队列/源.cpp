#include<iostream>
#include<stdexcept>
using namespace std;
//队列
template<typename T>
class Queue {
private:
    T* data;
    int front;
    int rear;//指向队尾元素的后一个位置
    int capacity;
    void resize();
public:
    Queue() :data(new T[10]), front(0), rear(0), capacity(10) {}
    ~Queue();
    void enqueue(T value);
    T dequeue();
    T getFront()const;
    int getSize() const;
};
template<typename T>
Queue<T>::~Queue() {
    delete[] data;
}
template<typename T>
void Queue<T>::resize() {
    T* newdata = new T[capacity * 2];
    for (int i = 0; i < rear; ++i) {
        newdata[i] = data[i];
    }
    delete[] data;
    data = newdata;
}
template<typename T>
void Queue<T>::enqueue(T value) {
    if (rear == capacity) {
        resize();
    }
    data[rear++] = value;
}
template<typename T>
T Queue<T>::dequeue() {
    if (front == rear) {
        throw std::underflow_error("Queue is empty");
    }
    return data[front++];
}
template<typename T>
T Queue<T>::getFront()const {
    if (front == rear) {
        throw std::underflow_error("Queue is empty");
    }
    return data[front];
}
template<typename T>
int Queue<T>::getSize()const {
    return rear - front;
}
int main() {
    Queue<int>que;
    que.enqueue(3);
    que.enqueue(4);
    cout << que.getFront() << ' ' << que.getSize() << endl;
    que.dequeue();
    cout << que.getFront() << ' ' << que.getSize() << endl;
}