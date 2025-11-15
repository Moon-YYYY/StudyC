#include<iostream>
using namespace std;
//Ç¶Ì×½á¹¹Ìå
struct Point
{
	double x, y;
};

struct circle
{
	Point pt;//Ç¶Ì×Point
	double radius;
};

struct circles
{
	int size;
	circle c[100];//Ç¶Ì×circle
};
int main()
{
	circle c;
	c.pt.x = 9;
	c.pt.y = 8;
	c.radius = 5;
	circles cs = {
		2,{
		{{9, 8}, 5},
		{{2, 1}, 1}
}
	};
	for (int i = 0; i < cs.size; ++i)
	{
		circle tmp = cs.c[i];
		cout << '(' << tmp.pt.x << ',' << tmp.pt.y << ')' << tmp.radius << endl;
	}
	return 0;
}
