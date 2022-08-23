/**
 * Author:  Devotes
 * Date:    2022/5/31
 *
 * FixLog:
 *
 */

#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "../common.h"
#include <cassert>

/**********************************************
 * 语义分析符号表
 * 1. 函数符号
 * 2. 数组符号栈
 * 3. 普通变量符号栈
 **********************************************/
struct FormalParaSymbol{
    Type type;
    std::vector<int> array_dim;

    bool operator==(const FormalParaSymbol& symbol);
};

struct FuncSymbol{
    typedef std::vector<FormalParaSymbol> Args;
    std::string    id;             /* 函数名称 */
    Type           ret_type;       /* 返回类型 */
    Args           args;           /* 形参列表 */

    bool operator==(const FuncSymbol& symbol);
};

struct CommonVarSymbol{
    std::string    id;                  /* 符号名称 */
    Type           type;                /* 符号类型 */
    bool           is_const{false};     /* 是否常量 */
    bool           can_calcu{false};    /* 是否能在编译时计算 */
    double         v;                   /* 编译时计算的值 */
    bool operator==(const CommonVarSymbol& symbol);
};

struct ArrayVarSymbol{
    typedef std::vector<int> ArrDims;
    std::string    id;             /* 符号名称 */
    Type           type;           /* 符号类型 */
    bool           is_const;       /* 是否常量 */
    ArrDims        arrDims;        /* 数组类型 */

    bool operator==(const ArrayVarSymbol& symbol);
};

/**********************************************
 * 作用域定义
 *  1. 维护一个当前作用域普通变量符号表
 *  2. 维护一个当前作用域数组变量符号表
 *  3. 维护一个指向父作用域的指针
 *  4. 维护一个指向儿子作用域的指针列表
 **********************************************/
class SyScope{
public:
    friend class SymbolTableManager;
    SyScope(const std::string& name):scope_name(name){}

private:
    SyScope*                parent{nullptr};
    std::vector<SyScope*>   childs;

    typedef std::map<std::string, ArrayVarSymbol>               ArraySymbolTable;
    typedef std::map<std::string, CommonVarSymbol>              VarSymbolTable;
    typedef std::set<std::string>                               FuncCallSymbol;

    std::string       scope_name;
    ArraySymbolTable  array_symbol_table;
    VarSymbolTable    var_symbol_table;
    FuncCallSymbol    funcCallSymbol;
};

/**********************************************
 * 符号表管理器
 * 管理一系列的符号表，包括函数符号，普通变量符号，数组变量
 * 符号，以及维护作用域。并提供插入和查询符号的接口
 **********************************************/
class SymbolTableManager{
public:
    SymbolTableManager();

    ~SymbolTableManager();

    /* 当前作用区域是否为全局作用区域 */
    bool isInGlobalScope();

    /* 进入新的符号表、离开当前符号表 */
    void entryCurrentSymbol(const std::string& name = "");
    void leaveCurrentSymbol();

    /* 查找数组符号，查找普通符号，查找函数符号 */
    bool isInCurrentVarST   (const std::string& id);
    bool isInVarST          (const std::string& id);
    bool isInCurrentArrayST (const std::string& id);
    bool isInArrayST        (const std::string& id);
    bool isInFuncST         (const std::string& id);
    bool isInFuncCallST     (const std::string& id);

    /* 获取对应符号 */
    CommonVarSymbol getCommonVarSymbol  (const std::string& id);
    ArrayVarSymbol  getArrayVarSymbol   (const std::string& id);
    FuncSymbol      getFuncVarSymbol    (const std::string& id);


    /* 添加符号 */
    void addVarSymbol       (const CommonVarSymbol& symbol);
    void addArrayVarSymbol  (const ArrayVarSymbol&  symbol);
    void addFuncSymbol      (const FuncSymbol&      symbol);
    void addFuncCallSymbol  (const std::string id);

    /* 打印符号表 */
    void printSymbolTable   (std::ostream& out = std::cout);

    /* 删除作用域 */
    void destroyScope(SyScope* &scope);


private:

    typedef std::vector<FuncSymbol> FuncSymbolTable;

    FuncSymbolTable             func_symbol_pool;
    SyScope*                    global_scope;
    SyScope*                    current_scope;
    int                         index{0};
};

#endif //COMPILER_SYMBOL_H
