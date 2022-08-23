//
// Created by 20580 on 2022/8/12.
//

#ifndef COMPILER_SYMBOL_TABLE_H
#define COMPILER_SYMBOL_TABLE_H

#include <string>
#include "../ast/ast.h"

/* 符号条目 */
struct Symbol{
    unsigned type : 1;              /* 符号的类型 */
    unsigned isArray : 1;           /* 是否为数组 */
    unsigned isConst : 1;           /* 是否为常量 */
    unsigned isAref : 2;            /* 是否是数组引用 */
    unsigned scodep : 8;            /* 作用域深度 */

    SmartExpr value{nullptr};       /* 对应的值 */
    std::string id;                 /* 符号名 */
};


/* 作用域 */
class SymScope{
public:
    SymScope() = default;
    /// 添加符号
    void addSymbol(const std::shared_ptr<Symbol>& symbol);
    /// 查找符号
    std::shared_ptr<Symbol> getSymbol(const std::string& id);
    std::shared_ptr<SymScope> parent{nullptr};
    /// 设置作用域深度
    void setScopeDeep(int dp){ deep = dp; }
    /// 获取作用域深度
    int getScopeDeep() const { return deep; }
private:
    int deep;   /* 作用域深度 全局 = 0， 函数 = 1 */
    std::map<std::string, std::shared_ptr<Symbol>> curSymbolTable{};
};



#endif //COMPILER_SYMBOL_TABLE_H
