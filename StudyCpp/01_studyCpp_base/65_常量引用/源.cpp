#include<iostream>
using namespace std;
//常量引用
struct S {
    int a, b, c, d, e;
};
void printfS(const S& e) {
    //e.a = 1200;//错误，常量引用不能修改对象的值
    cout << e.a << e.b << e.c << e.d << e.e << endl;
    cout << &e << endl;
}
int main() {
    int a;
    const int& b = a;
    //引用等于指针常量
    //常量引用等于常量指针常量
    S s = { 2, 5, 7, 9, 0 };
    printfS(s);
    cout << &s << endl;

    return 0;
}