#include<iostream>
#include<stdexcept>
//µ¥ÏòÁ´±í
using namespace std;
#define eletype int 
struct ListNode {
	eletype data;
	ListNode* next;
	ListNode(eletype x) :data(x), next(NULL) {}
};
class LinkList {
private:
	ListNode* head;
	int size;
public:
	LinkList() :head(NULL), size(0) {}
	~LinkList();
	void insert(int index, eletype value);
	void remove(int index);
	int getIndex(eletype value);
	int getSize();
	ListNode* getListNode(int index);
	void Updata(int index, eletype value);
};
LinkList::~LinkList() {
	ListNode* curr = head;
	while (curr) {
		ListNode* tmp = curr;
		curr = curr->next;
		delete tmp;
	}
}
int LinkList::getSize() {
	return size;
}
void LinkList::insert(int index, eletype value) {
	if (index < 0 || index > size) {
		throw underflow_error("index is invalid");
	}
	ListNode* pre = new ListNode(value);
	if (index == 0) {
		pre->next = head;
		head = pre;
	}
	else {
		ListNode* curr = head;
		for (int i = 0; i < index - 1; ++i) {
			curr = curr->next;
		}
		pre->next = curr->next;
		curr->next = pre;
	}
	++size;
}
void LinkList::remove(int index) {
	if (index < 0 || index > size - 1) {
		throw underflow_error("index is invalid");
	}
	ListNode* curr = head;
	if (index == 0) {
		ListNode* curr = head;
		head = head->next;
		delete curr;
	}
	else {
		for (int i = 0; i < index - 1; ++i) {
			curr = curr->next;
		}
		ListNode* tmp = curr->next;
		curr->next = tmp->next;
		delete tmp;
	}
	--size;
}
int LinkList::getIndex(eletype value) {
	ListNode* curr = head;
	for (int i = 0; curr != NULL; ++i) {
		if (curr->data == value) {
			return i;
		}
	}
	return -1;
}
ListNode* LinkList::getListNode(int index) {
	if (index < 0 || index > size - 1) {
		throw underflow_error("index is invalid");
	}
	ListNode* curr = head;
	for (int i = 0; i < index; ++i) {
		curr = curr->next;
	}
	return curr;
}
void LinkList::Updata(int index, eletype value) {
	if (index < 0 || index > size - 1) {
		throw underflow_error("index is invalid");
	}
	ListNode* curr = head;
	for (int i = 0; i < index; ++i) {
		curr = curr->next;
	}
	curr->data = value;
}
int main() {
	LinkList mylist;
	for (int i = 0; i < 10; ++i) {
		mylist.insert(i, i * 10);
	}
	for (int i = 0; i < mylist.getSize(); ++i) {
		cout << mylist.getListNode(i)->data << ' ';
	}
	cout << endl;

	mylist.remove(2);
	for (int i = 0; i < mylist.getSize(); ++i) {
		cout << mylist.getListNode(i)->data << ' ';
	}
	cout << endl;
	mylist.Updata(2, 1314);
	for (int i = 0; i < mylist.getSize(); ++i) {
		cout << mylist.getListNode(i)->data << ' ';
	}
	cout << endl;

	return 0;
}