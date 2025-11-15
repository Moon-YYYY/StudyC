#include <iostream>
using namespace std;
//二维数组
int main()
{
    //1、定义二维数组
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    int arr2[][4] = {//定义数组的时候，第一行的元素个数可以省略
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    int arr3[][4] = { 1,2,3,4,5,6,7,8,9,10,11,12 };

    //2、输出二维数组
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}