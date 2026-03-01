#include<iostream>
using namespace std;
//哈希表
template<typename KeyType, typename ValueType>
class HashNode {
public:
	KeyType key;
	ValueType value;
	HashNode* next;

	HashNode(const KeyType& key, const ValueType& value) {
		this->key = key;
		this->value = value;
		this->next = NULL;
	}
};
template<typename KeyType, typename ValueType>
class HashTable {
private:
	int size;
	HashNode<KeyType, ValueType>** table;
	int hash(const KeyType& key)const {
		int hashkey = key % size;
		if (hashkey < 0) {
			hashkey += size;
		}
		return hashkey;
	}
public:
	HashTable(int size = 256);
	~HashTable();
	void insert(const KeyType& key, const ValueType& value);
	void remove(const KeyType& key);
	bool find(const KeyType& key, ValueType& value)const;
};
template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(int size){
	this->size = size;
	this->table = new HashNode<KeyType, ValueType>* [size];
	for (int i = 0; i < size; ++i) {
		this->table[i] = NULL;
	}
}
template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::~HashTable() {
	for (int i = 0; i < size; ++i) {
		if (table[i]) {
			HashNode<KeyType, ValueType>* current = table[i];
			while (current) {
				HashNode<KeyType, ValueType>* next = current->next;
				delete current;
				current = next;
			}
			table[i] = NULL;
		}
	}
	delete table;
	table = NULL;
}
template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
	int index = hash(key);
	HashNode<KeyType, ValueType>* now = new HashNode<KeyType, ValueType>(key, value);
	if (table[index] == NULL) {
		table[index] = now;
	}
	else {
		now->next = table[index];
		table[index] = now;
	}
}
template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::remove(const KeyType& key) {
	int index = hash(key);
	if (table[index]) {
		if (table[index]->key == key) {
			HashNode<KeyType, ValueType>* next = table[index]->next;
			delete table[index];
			table[index] = next;
		}
		else {
			HashNode<KeyType, ValueType>* current = table[index];
			while (current->next && current->next->key != key) {
				current = current->next;
			}
			if (current->next) {
				HashNode<KeyType, ValueType>* next = current->next->next;
				delete current->next;
				current->next = next;
			}
		}
	}
}
template<typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::find(const KeyType& key, ValueType& value)const {
	int index = hash(key);
	if (table[index]) {
		if (table[index]->key == key) {
			value = table[index]->value;
			return true;
		}
		else {
			HashNode<KeyType, ValueType>* current = table[index];
			while (current->next && current->next->key != key) {
				current = current->next;
			}
			if (current->next) {
				value = current->next->value;
				return true;
			}
		}
	}
	return false;
}
int main() {
	HashTable<int, char>h(1000);
	h.insert(1, 'a');

	h.insert(2, 'b');

	h.insert(3, 'c');

	h.insert(41012012, 'd');
	char val;
	if (!h.find(43, val)) {
		cout << "43 notfind" << endl;
	}
	if (h.find(41012012, val)) {
		cout << "41012012 found, value is "<< val << endl;
	}

	return 0;
}