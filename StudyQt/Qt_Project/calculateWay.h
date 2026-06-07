/******************************************************************************
 * calculateWay.h
 * 计算引擎模块 - 头文件
 *
 * 本文件定义了计算器的核心计算逻辑类 CalculateWay。
 * 该类负责解析数学表达式字符串并执行四则运算（加、减、乘、除），
 * 同时支持百分号运算和正负号切换功能。
 *
 * 设计思路：
 *   - 将计算逻辑从 CalculatorWindow（界面层）中分离出来，形成独立的计算引擎层
 *   - 采用"表达式解析"模式，输入字符串，输出 double 结果
 *   - 支持运算符优先级：先乘除，后加减
 *   - 与 Qt 的 QString 字符串处理无缝集成
 *
 * 使用方式：
 *   CalculateWay calc;
 *   double result = calc.calculate("3+5×2");  // 返回 13.0
 *   double pct = calc.handlePercent(50);        // 返回 0.5
 *   double neg = calc.handlePlusMinus(10);       // 返回 -10
 ******************************************************************************/

#ifndef CALCULATEWAY_H       // 头文件保护宏：防止同一个头文件被多次包含
#define CALCULATEWAY_H         // 定义头文件保护宏（与上一行 #ifndef 配对使用）

#include <QString>            // Qt 字符串类，用于处理输入的表达式

/**
 * @class CalculateWay
 * @brief 计算引擎核心类
 *
 * 该类是计算器的"大脑"，负责所有数学运算的执行。
 * 采用面向对象设计，将计算逻辑封装在独立的类中，
 * 不与任何 UI 类耦合，便于单独测试和复用。
 *
 * 职责划分：
 *   - calculate()      ：对外接口，接收表达式字符串，返回计算结果
 *   - handlePercent()  ：百分号运算（÷100）
 *   - handlePlusMinus()：正负号切换（取反）
 *   - parseExpression()：内部方法，解析表达式并计算
 *
 * 表达式语法说明：
 *   - 支持的运算符：+（加）、-（减）、×（乘）、÷（除）
 *   - 支持负数，如 "-5+3"
 *   - 支持小数，如 "3.14×2"
 *   - 运算优先级：× 和 ÷ 优先于 + 和 -
 *   - 注意：当前版本不支括号
 */
class CalculateWay {
public:
    /**
     * @brief 构造函数
     *
     * 默认构造函数，当前无需初始化任何成员变量。
     * 所有计算都是无状态的——每次调用 calculate() 都是独立的，
     * 不会依赖或修改类内部状态。
     */
    CalculateWay();

    /**
     * @brief 析构函数
     *
     * 虚析构函数（虽然没有 virtual 关键字，但作为基类使用时建议加上）。
     * 当前无需释放任何资源。
     */
    ~CalculateWay();

    /**
     * @brief 执行表达式计算（对外主接口）
     * @param expression 数学表达式字符串
     *                   例如："3+5×2-4÷2" 或 "-5+3.14×2"
     * @return double 计算结果
     *
     * 功能说明：
     *   1. 接收用户输入的字符串表达式
     *   2. 调用 parseExpression() 进行解析和计算
     *   3. 返回 double 类型的计算结果
     *
     * 使用示例：
     *   CalculateWay calc;
     *   double r1 = calc.calculate("10+20");       // 30.0
     *   double r2 = calc.calculate("3×4+2");       // 14.0
     *   double r3 = calc.calculate("10÷3");        // 3.33333...
     */
    double calculate(const QString& expression);

    /**
     * @brief 百分号运算（÷100）
     * @param value 原始数值
     * @return double 除以 100 后的值
     *
     * 功能说明：
     *   在计算器中按下 "%" 按钮时调用。
     *   本质就是除以 100，例如：
     *     50 → 0.5
     *     200 → 2.0
     *     3.14 → 0.0314
     *
     * 注意：
     *   在一些高级计算器中，百分号的行为更复杂（如"200+10%"），
     *   但本实现采用最简单的"÷100"模式。
     */
    double handlePercent(double value);

    /**
     * @brief 正负号切换（±）
     * @param value 原始数值
     * @return double 取反后的值
     *
     * 功能说明：
     *   在计算器中按下 "±" 或 "(-)" 按钮时调用。
     *   本质就是乘以 -1，例如：
     *     5 → -5
     *     -3 → 3
     *     0 → 0
     */
    double handlePlusMinus(double value);

private:
    /**
     * @brief 解析并计算表达式（内部核心方法）
     * @param expression 要解析的表达式字符串
     * @param result     输出参数，用于返回计算结果
     * @return bool 解析是否成功
     *         - true  ：解析成功，result 中包含有效结果
     *         - false ：解析失败（如空字符串、非法字符等）
     *
     * 实现算法（两级扫描法）：
     *   第一轮：处理所有乘除运算（× 和 ÷）
     *     - 使用正则查找 "数字 × 数字" 或 "数字 ÷ 数字" 的模式
     *     - 逐对计算并替换，直到表达式中不再有乘除运算符
     *   第二轮：处理所有加减运算（+ 和 -）
     *     - 用同样的方式处理加减法
     *     - 最终剩余的就是计算结果
     *
     * 示例执行过程：
     *   输入："3+5×2-4÷2"
     *   第一轮（乘除）：
     *     "5×2" → 10  → "3+10-4÷2"
     *     "4÷2" → 2   → "3+10-2"
     *   第二轮（加减）：
     *     "3+10" → 13 → "13-2"
     *     "13-2" → 11 → "11"
     *   结果：11.0
     *
     * 边界情况：
     *   - 单个数字：直接转换为 double 返回
     *   - 除数为 0：返回 0 并标记成功（实际应返回错误）
     *   - 空字符串：返回 false
     *
     * 当前限制：
     *   - 不支持括号（需要更复杂的语法解析，如递归下降或调车场算法）
     *   - 不支持函数（如 sin、cos、sqrt 等）
     *   - 不支持连续运算符（如 "3++5"）
     */
    bool parseExpression(const QString& expression, double& result);
};

#endif // CALCULATEWAY_H