#include <iostream>
#include <iomanip>
using namespace std;
//关于设置输出小数点后的位数
int main() {
    double num = 1.1415926;
    double sum = 1.13141123;
    //cout << fixed; //设置输出为固定小数点
    cout << fixed << setprecision(4) << num << endl;//输出3.14，小数点后保留两位小数
    cout << defaultfloat;//通过此方法来取消输出为固定小数点的输出格式
    cout << setprecision(4) << sum << endl;//此时输出为默认的输出格式，小数点前后保留的有效数字之和为4
    return 0;
}