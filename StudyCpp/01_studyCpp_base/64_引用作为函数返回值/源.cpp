#include <iostream>
using namespace std;
//引用作为函数返回值
int& add(int& a, int& b) {
    a += b;
    return a;
}
int& list(int Arr[], int s) {//※数组作为函数参数时总是退化为指针，所以这里的Arr[]实际上是int*
    return Arr[s];
}
int main() {
    int x = 10, y = 20;
    int& z = add(x, y);
    int arr[] = {1, 2, 3, 4, 5};
    list(arr, 3) = 100;//相当于arr[3] = 100;
    cout << "arr[3] = " << arr[3] << endl;
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "z = " << z << endl;
    return 0;
}