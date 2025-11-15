#include <iostream>
using namespace std;
//break 关键字：用于跳出循环体，结束当前循环，在嵌套循环中使用break会跳出当前循环，但是外部的循环继续执行。
int main() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            cout << "j = " << j;
            if (j == 5) {
                break; //跳出当前循环
            }
        }
            cout << "i = " << i << endl;//i会一直输出到10，因为break只会跳出当前循环，不会影响外部循环的执行。
    }//会输出五行，每行输出j=0到j=5，然后输出i=0到i=9。
    return 0;
}