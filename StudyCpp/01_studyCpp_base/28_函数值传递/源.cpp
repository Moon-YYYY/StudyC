#include <iostream>
using namespace std;
//函数值的单向传递：函数值从主函数传递到外部函数只是拷贝了一份数据，外部函数不能把值修改然后传递到主函数
void swap(int a, int b) {
    int tmp = a; a = b; b = tmp;
}
int main()
{
    int x = 6, y = 9;
    swap(x, y); //调用函数
    cout << x << ' ' << y << endl; //输出结果:6 9，并未交换，因为只是传递了一份数据，没有传递地址。
    return 0;
}