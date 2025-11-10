#include<iostream>
using namespace std;
//do-while循环 和while循环的区别在于，while后面的括号里表达式判断为假的时候
//do-while循环至少执行一次循环体，而while循环则不执行循环体。
int main() {
    int i = 0;
    do {
        cout << i << " ";
        i++;
    } while (i < 5);//此时和while相同都是输出0 1 2 3 4
    cout << endl;
    i = 0;
    while (i) {
        cout << "i = " << i << " ";
        i++;
    }//此代码不会执行因为i为0，所以while循环条件永远为假，所以不会输出任何内容
    cout << endl;
    int a = 0;
    do {
        cout << "a = " << a << " ";
    } while (a);//此时a为0，所以do-while循环至少执行一次循环体，输出0
    return 0;
}