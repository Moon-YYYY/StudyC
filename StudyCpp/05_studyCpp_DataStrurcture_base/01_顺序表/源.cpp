#include<iostream>
#include<stdexcept>
//顺序表(多类型版本)
using namespace std;
template<typename T>
class List {
private:
	T* elements;
	int size;
	int capacity;
public:
	List() : elements(new T[10]), size(0), capacity(10) {}
	~List();
	void insert(int index, T value);
	void remove(int index);
	int getsize();
	bool isempty();
	T find(int index);
	void printList();
	void Updata(int index, T value);
};
template<typename T>
List<T>::~List() {
	delete[] elements;
}
template<typename T>
void List<T>::insert(int index, T value) {
	if (index < 0 || index > size) {
		throw underflow_error("index is invalid");
	}
	if (size == capacity) {
		T* newelements = new T[capacity * 2];
		for (int i = 0; i < size; ++i) {
			newelements[i] = elements[i];
		}
		delete[] elements;
		elements = newelements;
	}
	for (int i = size; i > index; --i) {
		elements[i] = elements[i - 1];
	}
	elements[index] = value;
	++size;
}
template<typename T>
void List<T>::remove(int index) {
	if (index < 0 || index > size - 1) {
		throw underflow_error("index is invalid");
	}
	for (int i = index; i < size; ++i) {
		elements[i] = elements[i + 1];
	}
	--size;
}
template<typename T>
int List<T>::getsize() {
	return size;
}
template<typename T>
bool List<T>::isempty() {
	return size == 0;
}
template<typename T>
T List<T>::find(int index) {
	return elements[index];
}
template<typename T>
void List<T>::printList() {
	for (int i = 0; i < size; ++i) {
		cout << find(i) << ' ';
	}
	cout << endl;
}
template<typename T>
void List<T>::Updata(int index, T value) {
	elements[index] = value;
}
int main() {
	List<int>mylist;
	cout << "isempty = " << mylist.isempty() << endl;
	for (int i = 0; i < 20; ++i) {
		mylist.insert(i, i * 10);
	}
	mylist.printList();
	mylist.remove(2);
	mylist.printList();
	mylist.Updata(3, 1314);
	mylist.printList();
	cout << mylist.getsize() << endl;
	return 0;
}