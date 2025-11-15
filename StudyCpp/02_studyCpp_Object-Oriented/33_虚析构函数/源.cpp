//虚析构函数的示例代码
#include <iostream>
using namespace std;

class Base {
public:
    Base() {
        cout << "Base构造函数" << endl;  // 基类构造函数
    }
    virtual ~Base() {  // 虚析构函数
        cout << "Base虚析构函数" << endl;  // 基类析构函数
    }
};

class Derived : public Base {
public:
    Derived() {
        data = new int(100);  // 派生类分配动态内存
        cout << "Derived构造函数" << endl;  // 派生类构造函数
    }
    ~Derived() override {  // 派生类析构函数，override确保正确重写
        delete data;  // 释放派生类分配的内存
        cout << "Derived析构函数" << endl;  // 派生类析构函数
    }
private:
    int* data;  // 派生类特有的数据成员
};

int main() {
    // 虚析构函数的delete操作
    Base* obj = new Derived();  // 基类指针指向派生类对象
    cout << "准备delete对象..." << endl;  // 提示信息
    delete obj;  // 正确调用派生类和基类的析构函数（多态析构）

    // 也可以直接创建派生类对象（自动管理）
    cout << "=== 直接创建派生类对象 ===" << endl;  // 分隔线
    Derived derivedObj;  // 自动调用构造函数和析构函数
    cout << "main函数结束" << endl;  // 程序结束提示
    return 0;  // 程序正常退出
}
/*
Base构造函数
Derived构造函数
准备delete对象...
Derived析构函数
Base虚析构函数
=== 直接创建派生类对象 ===
Base构造函数
Derived构造函数
main函数结束
Derived析构函数
Base虚析构函数
*/
