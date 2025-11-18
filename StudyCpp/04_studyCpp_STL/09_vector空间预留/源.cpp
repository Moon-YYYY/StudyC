#include<iostream>
#include<vector>
using namespace std;
//vector空间预留
int main() {
	vector<int>v;
	v.reserve(100);//提前扩容成100个单位大小的容器
	for (int i = 0; i < 100; ++i) {
		cout << "size = " << v.size() << " , " << "capacity = " << v.capacity() << endl;
		v.push_back(i);//频繁加数据会涉及到频繁扩容，此时提前扩容来减小时间损耗
	}
	//reserve修改的是capacity
	//resize修改的是size
	return 0;
}