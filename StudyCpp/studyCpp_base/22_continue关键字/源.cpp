#include <iostream>
using namespace std;
//continue :直接结束本次循环进入下一次循环，但并不是跳出整个循环。
//break :直接结束整个循环，后续的语句不会执行。
int main() {
	for (int i = 0; i < 10; ++i) {
		if(i < 3)
			continue;
		cout << "i = " << i << endl;//输出i=3,4,5,6,7,8,9，跳过了i=0,1,2，因为continue直接结束本次循环进入下一次循环。
	}
	return 0;
}