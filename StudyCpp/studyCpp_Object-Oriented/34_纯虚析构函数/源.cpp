#include <iostream>
using namespace std;

class Base {
public:
    Base() {
        cout << "Base构造函数" << endl;  // 基类构造函数
    }
    virtual ~Base() = 0;  // 纯虚析构函数声明，使Base成为抽象类
};

// 纯虚析构函数必须在类外定义
Base::~Base() {
    cout << "Base纯虚析构函数" << endl;  // 纯虚析构函数的实现
}

class Derived : public Base {
public:
    Derived() {
        data = new int(200);  // 派生类分配动态内存
        cout << "Derived构造函数" << endl;  // 派生类构造函数
    }
    ~Derived() override {  // 派生类析构函数
        delete data;  // 释放派生类分配的内存
        cout << "Derived析构函数" << endl;  // 派生类析构函数
    }
private:
    int* data;  // 派生类特有的数据成员
};

int main() {
    // 纯虚析构函数的delete操作
    Base* obj = new Derived();  // 基类指针指向派生类对象（多态）
    cout << "准备delete对象..." << endl;  // 提示信息
    delete obj;  // 正确调用派生类和基类的析构函数

    // 注意：不能直接创建Base对象，因为它是抽象类
    // Base baseObj;  // 这行会编译错误，因为Base是抽象类

    // 只能创建派生类对象
    cout << "=== 直接创建派生类对象 ===" << endl;  // 分隔线
    Derived derivedObj;  // 直接创建派生类对象

    cout << "main函数结束" << endl;  // 程序结束提示
    return 0;  // 程序正常退出
}
/*
Base构造函数
Derived构造函数
准备delete对象...
Derived析构函数
Base纯虚析构函数
=== 直接创建派生类对象 ===
Base构造函数
Derived构造函数
main函数结束
Derived析构函数
Base纯虚析构函数
*/