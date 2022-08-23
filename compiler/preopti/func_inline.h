//
// Created by 20580 on 2022/8/12.
//

#ifndef COMPILER_FUNC_INLINE_H
#define COMPILER_FUNC_INLINE_H

#include "../ast_visitor.h"
#include "symbol_table.h"
#include <map>
#include <set>

/* *
 * 函数内联的方法。
 *      分析
 *          1. 分析函数的参数中是否有数组， 有数组不展开
 *          2. 分析函数中是否有对函数赋值， 有赋值不展开
 *          3. 分析函数中的语句体量，体量复杂，不展开，一条简单的return语句展开
 *          4. 分析函数中是否有对全局数组或者全局变量的赋值，有赋值不展开
 *          6. 分析函数中是否有标准库函数的调用，有调用不展开
 *          7， 分析函数中是否有返回值，无返回值不展开
 *          构建函数的调用链关系
 *      执行函数内联
 *          深度优先遍历，展开函数
 * */
class FuncInline : public AstVisitor {
public:
    FuncInline() = default;
    void visit(const std::shared_ptr<CodeBlockNode>         &node) override;
    void visit(const std::shared_ptr<CompUnitNode>          &node) override;
    void visit(const std::shared_ptr<FuncCallNode>          &node) override;
    void visit(const std::shared_ptr<ArrayVarDef>           &node) override;
    void visit(const std::shared_ptr<FuncDef>               &node) override;
    void visit(const std::shared_ptr<VarDef>                &node) override;
    void visit(const std::shared_ptr<ReturnStmtNode>        &node) override;
    void visit(const std::shared_ptr<ArrayRefLValueNode>    &node) override;
    void visit(const std::shared_ptr<CommonLValueNode>      &node) override;
    void visit(const std::shared_ptr<AssignStmtNode>        &node) override;
    void visit(const std::shared_ptr<ExpressionNode>        &node) override;
    void visit(const std::shared_ptr<IfStmtNode>            &node) override;
    void visit(const std::shared_ptr<WhileStmtNode>         &node) override;
    void visit(const std::shared_ptr<ExpressionStmtNode>    &node) override;
    void visit(const std::shared_ptr<UnaryExpNode>          &node) override;
    void visit(const std::shared_ptr<BinaryOpExprNode>      &node) override;
    void visit(const std::shared_ptr<InitializedNode>       &node) override;
    void visit(const std::shared_ptr<IntValueNode>          &node) override{}
    void visit(const std::shared_ptr<FloatValueNode>        &node) override{}
    void visit(const std::shared_ptr<ContinueStmtNode>      &node) override{}
    void visit(const std::shared_ptr<CommonFormalParaNode>  &node) override{}
    void visit(const std::shared_ptr<BreakStmtNode>         &node) override{}
    void visit(const std::shared_ptr<ArrayFormalParaNode>   &node) override{}
private:
    /* 判断表达式的类型 */
    Type getType(const SmartExpr& expr);

    /* 扫描语句 */
    void gotoVisitStmt(const SmartNode& node);
    /* 进入一个新的作用域 */
    void enterScope();
    /* 离开作用域 */
    void leaveScope();

    /* 设置一个强制类型转换标记 */
    void setMustConvertToExpr(const SmartExpr& expr);

    /* 执行内联 */
    typedef const std::shared_ptr<FuncDef>& FuncDefPtr;
    typedef const std::shared_ptr<FuncCallNode>& FuncCallPtr;
    typedef std::set<std::string> CallSet;
    void funcInline();
    bool funcInline(FuncDefPtr defLoc, FuncCallPtr callLoc, CallSet cset);
    void executeInline(FuncDefPtr defLoc, FuncCallPtr callLoc);

    /* 扫描表达式，形参替换实参 */
    void scanExprToReplacePara(const SmartExpr& node, std::map<std::string, SmartExpr>& p2pMap);

    /* 表达式替换函数调用 */
    void exprReplaceFuncCall(const SmartExpr& node, const SmartExpr& funcCall);

    std::shared_ptr<FuncDef> currentFunc;                                               /* 当前函数指针 */
    std::shared_ptr<FuncDef> mainFunc;                                                  /* 主函数AST结点指针，展开操作从main函数开始由下往上展开 */
    std::map<std::string, std::shared_ptr<FuncDef>> funcDefMap;                         /* 函数def的map */
    const static std::set<std::string> stdFuncSet;                                      /* 标准库函数调用集合 */
    std::shared_ptr<SymScope> curScopePtr{nullptr};                                     /* 当前作用域的指针 */
    int deep{0};                                                                        /* 作用域深度 */
    std::set<SmartNode> funcCanDelete;                                                  /* 展开后要删除的函数列表 */
};



#endif //COMPILER_FUNC_INLINE_H
