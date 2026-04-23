#include "calculateWay.h"
#include <QStringList>
#include <QRegularExpression>

/**
 * @brief 构造函数
 */
CalculateWay::CalculateWay() {
}

/**
 * @brief 析构函数
 */
CalculateWay::~CalculateWay() {
}

/**
 * @brief 执行计算
 * @param expression 表达式字符串
 * @return 计算结果
 */
double CalculateWay::calculate(const QString& expression) {
    double result = 0;
    parseExpression(expression, result);
    return result;
}

/**
 * @brief 处理百分号
 * @param value 值
 * @return 处理后的值
 */
double CalculateWay::handlePercent(double value) {
    return value / 100.0;
}

/**
 * @brief 处理正负号
 * @param value 值
 * @return 处理后的值
 */
double CalculateWay::handlePlusMinus(double value) {
    return -value;
}

/**
 * @brief 解析表达式
 * @param expression 表达式字符串
 * @param result 计算结果
 * @return 是否解析成功
 */
bool CalculateWay::parseExpression(const QString& expression, double& result) {
    // 移除空格
    QString expr = expression.trimmed();
    
    // 处理单个数字
    bool ok;
    result = expr.toDouble(&ok);
    if (ok) {
        return true;
    }
    
    // 处理运算优先级：先处理乘除，再处理加减
    // 处理乘除运算（×和÷）
    // 正则表达式解释：
    // (-?[\d.]+)：匹配第一个数字，-?表示可选的负号，[\d.]+表示一个或多个数字或小数点
    // ([×÷])：匹配乘除运算符
    // (-?[\d.]+)：匹配第二个数字，同样支持负数
    QRegularExpression regMulDiv("(-?[\\d.]+)([×÷])(-?[\\d.]+)");
    QRegularExpressionMatch match;
    
    // 循环处理所有乘除运算，直到没有匹配为止
    // 每次循环会：
    // 1. 找到表达式中的第一个乘除运算
    // 2. 计算结果
    // 3. 用结果替换原表达式中的乘除运算部分
    while ((match = regMulDiv.match(expr)).hasMatch()) {
        // 从匹配结果中提取第一个数字
        double num1 = match.captured(1).toDouble();
        // 从匹配结果中提取运算符
        QString op = match.captured(2);
        // 从匹配结果中提取第二个数字
        double num2 = match.captured(3).toDouble();
        // 初始化子结果变量
        double subResult = 0;
        
        if (op == "×") {
            subResult = num1 * num2;
        } else if (op == "÷") {
            if (num2 != 0) {
                subResult = num1 / num2;
            } else {
                result = 0; // 除数为0，返回0
                return true;
            }
        }
        
        // 替换计算结果
        expr.replace(match.captured(0), QString::number(subResult));
    }
    
    // 处理加减
    QRegularExpression regAddSub("(-?[\\d.]+)([+\\-])(-?[\\d.]+)");
    
    while ((match = regAddSub.match(expr)).hasMatch()) {
        double num1 = match.captured(1).toDouble();
        QString op = match.captured(2);
        double num2 = match.captured(3).toDouble();
        double subResult = 0;
        
        if (op == "+") {
            subResult = num1 + num2;
        } else if (op == "-") {
            subResult = num1 - num2;
        }
        
        // 替换计算结果
        expr.replace(match.captured(0), QString::number(subResult));
    }
    
    // 最终结果
    result = expr.toDouble(&ok);
    return ok;
}
