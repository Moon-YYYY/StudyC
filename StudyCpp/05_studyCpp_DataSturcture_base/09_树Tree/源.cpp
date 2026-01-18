//Tree
#include<iostream>
using namespace std;
template<typename T>
struct ListNode{
	T data;
	ListNode* next;
	ListNode (T d) : data(d), next(NULL) {}
};
template<typename T>
struct TreeNode {
	T data;
	TreeNode() {
		childrenHead = NULL;
	}
	ListNode<TreeNode<T>*>* childrenHead;
	void Addchild(TreeNode<T>* node) {
		ListNode<TreeNode<T>*>* childNode = new ListNode<TreeNode<T>*>(node);
		if (childrenHead == NULL) {//如果头节点是空的，那就让孩子节点变成头节点（头插法）
			childrenHead = childNode;
		}
		else {
			childNode->next = childrenHead;
			childrenHead = childNode;
		}
	}
};
template<typename T>
class Tree {
private:
	TreeNode<T>* nodes;//树的节点的集合，其实是个顺序表
	TreeNode<T>* root;//根节点的指针
public:
	Tree();
	Tree(int maxNodes);//指定最大节点的构造函数
	~Tree();
	TreeNode<T>* GetTreeNode(int id);//获取树的节点
	void setRoot(int rootID);//设置根节点
	void Addchild(int parentID, int sonID);//添加节点，指定父节点和子节点
	void AssignData(int nodeID, T data);//把这个节点ID给他的数据域置数
	void Print(TreeNode<T>* node = NULL);//打印树
};
template<typename T>
Tree<T>::Tree() {
	nodes = new TreeNode<T>[100001];
}
template<typename T>
Tree<T>::Tree(int maxNodes) {
	nodes = new TreeNode<T>[maxNodes];
}
template<typename T>
Tree<T>::~Tree() {
	delete[] nodes;
}
template<typename T>
TreeNode<T>* Tree<T>::GetTreeNode(int id) {
	return &nodes[id];
}
template<typename T>
void Tree<T>::setRoot(int rootID) {
	root = GetTreeNode(rootID);
}
template<typename T>
void Tree<T>::Addchild(int parentID, int sonID) {
	TreeNode<T>* parentNode = GetTreeNode(parentID);
	TreeNode<T>* sonNode = GetTreeNode(sonID);
	parentNode->Addchild(sonNode);
}
template<typename T>
void Tree<T>::AssignData(int nodeID, T data) {
	GetTreeNode(nodeID)->data = data;
}
template<typename T>
void Tree<T>::Print(TreeNode<T>* node) {
	if (node == NULL) {
		node = root;
	}
	cout << node->data;
	ListNode<TreeNode<T>*>* tmp = node->childrenHead;
	while (tmp) {
		Print(tmp->data);
		tmp = tmp->next;
	}
}
int main() {
	Tree<char>T(9);
	T.setRoot(0);
	T.AssignData(0, 'a');
	T.AssignData(1, 'b');
	T.AssignData(2, 'c');
	T.AssignData(3, 'd');
	T.AssignData(4, 'e');
	T.AssignData(5, 'f');
	T.AssignData(6, 'g');
	T.AssignData(7, 'h');
	T.AssignData(8, 'i');

	T.Addchild(0, 2);
	T.Addchild(0, 1);

	T.Addchild(1, 3);

	T.Addchild(2, 5);
	T.Addchild(2, 4);

	T.Addchild(3, 8);
	T.Addchild(3, 7);
	T.Addchild(3, 6);

	T.Print();



	return 0;
}