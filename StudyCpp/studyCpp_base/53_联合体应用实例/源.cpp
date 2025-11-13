#include <iostream>
#include<string.h>
using namespace std;
//联合体应用实例
struct info {
	char _name[20];
	int _role;

	union {
		double score;
		char course[20];
	}sc;//sc是union的初始化名字，也可以在union后面加上一个自定义类型
	info(const char name[20], int role, double s, const char c[20]) {
		cout << (int)name << endl;
		strcpy_s(_name, name);
		_role = role;
		if (s > 0) {
			sc.score = s;
		}
		if (strlen(c) > 0) {
			strcpy_s(sc.course, c);
		}
	}
};
int main() {
	info a[4] =
	{
		info("周老师", 0, -1, "cgaga"),
		info("周老师", 0, -1, "python"),
		info("周同学", 1, 90, ""),
		info("肖同学", 1, 98, ""),
	};

	for (int i = 0; i < 4; ++i)
	{
		if (a[i]._role == 0)
		{
			cout << a[i]._name << "是一位老师, 他是教" << a[i].sc.course/*course是union里面的，所以要找sc里面的course要进行两次访问*/ << "的" << endl;
		}
		else if (a[i]._role != 0)
		{
			cout << a[i]._name << "是一位同学，他的分数是" << a[i].sc.score << endl;
		}
	}
	return 0;

}