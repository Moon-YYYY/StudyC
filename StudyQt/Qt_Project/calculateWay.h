#ifndef CALCULATEWAY_H
#define CALCULATEWAY_H

#include <QString>

/**
 * @class CalculateWay
 * @brief 计算逻辑类
 * 
 * 该类负责处理计算器的各种计算操作
 */
class CalculateWay {
public:
    /**
     * @brief 构造函数
     */
    CalculateWay();
    
    /**
     * @brief 析构函数
     */
    ~CalculateWay();
    
    /**
     * @brief 执行计算
     * @param expression 表达式字符串
     * @return 计算结果
     */
    double calculate(const QString& expression);
    
    /**
     * @brief 处理百分号
     * @param value 值
     * @return 处理后的值
     */
    double handlePercent(double value);
    
    /**
     * @brief 处理正负号
     * @param value 值
     * @return 处理后的值
     */
    double handlePlusMinus(double value);

private:
    /**
     * @brief 解析表达式
     * @param expression 表达式字符串
     * @param result 计算结果
     * @return 是否解析成功
     */
    bool parseExpression(const QString& expression, double& result);
};

#endif // CALCULATEWAY_H
